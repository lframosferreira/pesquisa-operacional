using JuMP
using HiGHS

mutable struct coloracaoData
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
	return coloracaoData(numberOfVertices, adjMatrix)
end

model = Model(HiGHS.Optimizer)

file = open(ARGS[1], "r")

data = readData(file)

# Podemos assumir que existem N cores. Ci indica que a cor i está sendo utilizada
@variable(model, c[i = 1:data.numberOfVertices], Bin)

# Vértice i está colorido com a cor j
@variable(model, x[i = 1:data.numberOfVertices, j = 1:data.numberOfVertices], Bin)


# Cada vértice deve estar colorido com exatamente uma cor
for i in 1:data.numberOfVertices
	@constraint(model, sum(x[i, j] for j in 1:data.numberOfVertices) == 1)
end

# Se a aresta ij existe em E(G), as cores de i e j devem ser diferentes
for i in 1:data.numberOfVertices
	for j in 1:data.numberOfVertices
		if (data.adjMatrix[i, j] == 1)
			for color in 1:data.numberOfVertices
				@constraint(model, x[i, color] + x[j, color] <= c[color])
			end
		end
	end
end


# Minimizar o número de cores utilizadas
@objective(model, Min, sum(c[i] for i ∈ 1:data.numberOfVertices))

print(model)

optimize!(model)

sol = objective_value(model)
println("TP1 2019022553 = ", sol)
