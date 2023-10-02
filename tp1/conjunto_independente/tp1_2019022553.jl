using JuMP
using HiGHS

mutable struct conjuntoIndependenteData
	numberOfVertices::Int64 # número de vértices
	adjMatrix::Matrix{Int64} # Matriz de adjacências
end

function readData(file)
	numberOfVertices = 0
	adjMatrix = missing
	for line in eachline(file)
		q = split(line, "\t")
		if (q[1] == "n")
			numberOfVertices = parse(Int64, q[2])
			adjMatrix = zeros(Int64, numberOfVertices, numberOfVertices)
		elseif q[1] == "e"
			v1 = parse(Int64, q[2])
            v2 = parse(Int64, q[3])
			adjMatrix[v1, v2] = 1
            adjMatrix[v2, v1] = 1
		end
	end
	return conjuntoIndependenteData(numberOfVertices, adjMatrix)
end

model = Model(HiGHS.Optimizer)

file = open(ARGS[1], "r")

data = readData(file)

# Vértice está no conjunto independente
@variable(model, x[i = 1:data.numberOfVertices], Bin)

# Se a aresta ij estiver no grafo, apenas um dos vértices pode estar no conjunto independente
for i in 1:data.numberOfVertices
    for j in i:data.numberOfVertices
        if (data.adjMatrix[i, j] == 1)
	        @constraint(model, x[i] + x[j] <= 1)
        end
    end
end

# Maximizar o número de vértices no conjunto independente
@objective(model, Max, sum(x[i] for i ∈ 1:data.numberOfVertices))

print(model)

optimize!(model)

sol = objective_value(model)
println("Valor otimo = ", sol)
