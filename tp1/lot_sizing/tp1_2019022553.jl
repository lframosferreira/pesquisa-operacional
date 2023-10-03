using JuMP
using HiGHS

mutable struct lotSizingData
  n::Int64 # número de períodos
  productionCosts::Array{Int64} # vetor de custos de produção por período
  demands::Array{Int64} # vetor de demanda por período
  storageCost::Array{Int64} # vetor de custos de estocagem por período
  assessmentCost::Array{Int64} # vetor de custos das multas por período

end

function readData(file)
  print("readData")
end

model = Model(HiGHS.Optimizer)

file = open(ARGS[1], "r")

data = readData(file)

@objective(model, Min, 3)

print(model)

optimize!(model)

sol = objective_value(model)
println("TP1 2019022553 = ", sol)

