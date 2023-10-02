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
	for i ∈ 1:data.n
		if value(x[i]) > 0.5
			println("$i")
		end
	end
	println()
end

model = Model(HiGHS.Optimizer)

file = open(ARGS[1], "r")

data = readData(file)

@variable(model, x[i = 1:data.n][i = 1:data.n], Bin)

# cada objeto deve estar em exatamente uma caixa
for i in 1:data.n
	@constraint(model, sum(x[i][j] for j in 1:data.n) == 1)
end

# soma nas caixas menor que 20
for j in 1:data.n
	@constraint(model, sum(data.weights[i] * x[i][j] for i ∈ 1:data.n) <= 20)
end

@objective(model, Min, sum(x[i] for i ∈ 1:data.n))

print(model)

optimize!(model)

sol = objective_value(model)
println("Valor otimo= ", sol)