using JuMP
using HiGHS

mutable struct empacotamentoData
	numberOfObjects::Int # número de objetos
	weights::Array{Int} # Pesos dos objetos
end

function readData(file)
	numberOfObjects = 0
	weights = []
	q = split(line, "\t")
	if (q[1] == "n")
		n = parse(Int64, q[2])
		weights = zeros(Int64, n)
	elseif q[1] == "o"
		id = parse(Int64, q[2])
		weight = parse(Float64, q[3])
		weights[id] = weight
	end
	return empacotamentoData(numberOfObjects, weights)
end

function printSolution(data, x)
	println("Uma Cobertura Mínima:")
	for i = 1: data.n
		if value(x[i]) > 0.5
			println("$i")
		end
	end
	println()
end

model = Model(HiGHS.Optimizer)

file = open(ARGS[1], "r")

data = readData(file)

@variable(model, x[i=1:data.n], Bin)

for i=1:data.n
	@constraint(model, sum(x[j] for j in data.ng[i]) >= 1)	
end