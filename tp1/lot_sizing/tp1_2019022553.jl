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
@variable(model, prod[i=1:data.numberOfPeriods])

# est[i] é o estoque do período i - 1 pro periodo i, isto é, quanto eu tenho em estoque pro período i
# assume-se que p[0] é 0
@variable(model, est[i=1:data.numberOfPeriods])

# debt[i] é o quanto estou devendo ao acabar o período i
@variable(model, est[i=1:data.numberOfPeriods])

@objective(model, Min, 3)

optimize!(model)

sol = objective_value(model)
println("TP1 2019022553 = ", sol)

