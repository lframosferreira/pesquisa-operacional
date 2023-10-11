using JuMP
using HiGHS

mutable struct lotSizingData
  numberOfPeriods::Int64 # número de períodos
  productionCosts::Array{Int64} # vetor de custos de produção por período
  demands::Array{Int64} # vetor de demanda por período
  storageCost::Array{Int64} # vetor de custos de estocagem por período
  assessmentCost::Array{Int64} # vetor de custos das multas por período

end

function readData(file)
  numberOfPeriod = missing
  productionCost = missing
  demand = missing
  storageCost = missing
  assessmentCost = missing
  for line in eachline(file)
    q = line.split("\t")
    if q[1] == "n"
      numberOfPeriods = parse(Int64, q[2])
      productionCost = zeros(Float64, numberOfPeriods)
      demand = zeros(Float64, numberOfPeriods)
      storageCost = zeros(Float64, numberOfPeriods)
      assessmentCost = zeros(Float64, numberOfPeriods)
    elseif q[1] == "c"
        productionCost[parse(Int64, q[2])] = q[3] 
    else
    end
  end
end

model = Model(HiGHS.Optimizer)

file = open(ARGS[1], "r")

data = readData(file)

@objective(model, Min, 3)

print(model)

optimize!(model)

sol = objective_value(model)
println("TP1 2019022553 = ", sol)

