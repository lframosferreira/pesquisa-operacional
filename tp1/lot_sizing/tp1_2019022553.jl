using JuMP
using HiGHS

mutable struct lotSizingData
  numberOfPeriods::Int64 # número de períodos
  productionCost::Array{Int64} # vetor de custos de produção por período
  demand::Array{Int64} # vetor de demanda por período
  storageCost::Array{Int64} # vetor de custos de estocagem por período
  assessmentCost::Array{Int64} # vetor de custos das multas por período

end

function readData(file)
  numberOfPeriods = missing
  productionCost = missing
  demand = missing
  storageCost = missing
  assessmentCost = missing
  for line in eachline(file)
    q = split(line, "\t")
    if q[1] == "n"
      numberOfPeriods = parse(Int64, q[2])
      productionCost = zeros(Float64, numberOfPeriods)
      demand = zeros(Float64, numberOfPeriods)
      storageCost = zeros(Float64, numberOfPeriods)
      assessmentCost = zeros(Float64, numberOfPeriods)
    elseif q[1] == "c"
      productionCost[parse(Int64, q[2])] = parse(Float64, q[3])
    elseif q[1] == "d"
      demand[parse(Int64, q[2])] = parse(Float64, q[3])
    elseif q[1] == "s"
      storageCost[parse(Int64, q[2])] = parse(Float64, q[3])
    elseif q[1] == "p"
      assessmentCost[parse(Int64, q[2])] = parse(Float64, q[3])
    end
  end
  return lotSizingData(numberOfPeriods, productionCost, demand, storageCost, assessmentCost)
end

model = Model(HiGHS.Optimizer)

file = open(ARGS[1], "r")

data = readData(file)

# prod[i] é a produção do período i
@variable(model, prod[i=1:data.numberOfPeriods] >= 0)

# est[i] é o quanto eu tenho em estoque pro período i
# assume-se que est[0] é 0
@variable(model, est[i=1:data.numberOfPeriods+1] >= 0)

# devo[i] é o quanto estou devendo no periodo i
@variable(model, devo[i=1:data.numberOfPeriods+1] >= 0)

# pago[i] é o quanto paguei no período i
@variable(model, pago[i=1:data.numberOfPeriods] >= 0)

# nunca posso pagar mais do que devo somado a demanda atual (teria estoque negativo)
for i in 1:data.numberOfPeriods
  @constraint(model, pago[i] <= devo[i] + data.demand[i])
end

# nunca posso pagar mais do que tenho
for i in 1:data.numberOfPeriods
  @constraint(model, pago[i] <= prod[i] + est[i])
end

@constraint(model, devo[1] == 0)
@constraint(model, est[1] == 0)
@constraint(model, devo[data.numberOfPeriods+1] == 0)

# est pro proximo periodo é prod + est - pago
for i in 1:data.numberOfPeriods
  @constraint(model, est[i+1] == prod[i] + est[i] - pago[i])
end

# devo é qnt devia antes + demanda menos pago
for i in 1:data.numberOfPeriods
  @constraint(model, devo[i+1] == devo[i] + data.demand[i] - pago[i])
end

@objective(model, Min, sum(prod[i] * data.productionCost[i] + est[i+1] * data.storageCost[i] + devo[i] * data.assessmentCost[i] for i in 1:data.numberOfPeriods))

optimize!(model)

sol = objective_value(model)
println("TP1 2019022553 = ", sol)

for i in 1:data.numberOfPeriods
  println("$(value(prod[i]))")
end

