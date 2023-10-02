using JuMP
using HiGHS

mutable struct empacotamentoData
	numberOfObjects::Int64 # número de objetos
	weights::Array{Float64} # Pesos dos objetos
end

function readData(file)
	numberOfObjects = 0
	weights = []
	for line in eachline(file)
		q = split(line, "\t")
		if (q[1] == "n")
			numberOfObjects = parse(Int64, q[2])
			weights = zeros(Float64, numberOfObjects)
		elseif q[1] == "o"
			id = parse(Int64, q[2])
			weight = parse(Float64, q[3])
			weights[id + 1] = weight
		end
	end
	return empacotamentoData(numberOfObjects, weights)
end

function printSolution(data, x)
	println("Empacotamento:")
	for i in 1:data.numberOfObjects
		if (y[i] == 1)
			print("CAIXA $i :")
			for j in 1:data.numberOfObjects
				println("$i $j ->")
			end
		end
	end
	println()
end

model = Model(HiGHS.Optimizer)

file = open(ARGS[1], "r")

data = readData(file)

# obj i na caixa j
@variable(model, x[i = 1:data.numberOfObjects, j = 1:data.numberOfObjects], Bin)

# caixa i sendo usada
@variable(model, y[i = 1:data.numberOfObjects], Bin)

# cada objeto deve estar em exatamente uma caixa
for i in 1:data.numberOfObjects
	@constraint(model, sum(x[i, j] for j in 1:data.numberOfObjects) == 1)
end

# soma nas caixas menor que 20
for j in 1:data.numberOfObjects
	@constraint(model, sum(data.weights[i] * x[i, j] for i ∈ 1:data.numberOfObjects) <= 20 * y[j])
end

@objective(model, Min, sum(y[i] for i ∈ 1:data.numberOfObjects))

print(model)

optimize!(model)

sol = objective_value(model)
println("Valor otimo = ", sol)
