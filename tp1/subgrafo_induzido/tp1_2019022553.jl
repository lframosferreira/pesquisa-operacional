using JuMP
using HiGHS

mutable struct subgrafoInduzidoData
	numberOfVertices::Int64 # número de vértices
	adjMatrix::Matrix{Int64} # Matriz de adjacências
    weightsMatrix::Matrix{Float64}
end

function readData(file)
	numberOfVertices = 0
	adjMatrix = missing
    weightsMatrix = missing;
	for line in eachline(file)
		q = split(line, "\t")
		if (q[1] == "n")
			numberOfVertices = parse(Int64, q[2])
            adjMatrix = zeros(Int64, numberOfVertices, numberOfVertices)
			weightsMatrix = zeros(Float64, numberOfVertices, numberOfVertices)
		elseif q[1] == "e"
			v1 = parse(Int64, q[2])
            v2 = parse(Int64, q[3])
            w = parse(Float64, q[4])
			adjMatrix[v1, v2] = 1
            adjMatrix[v2, v1] = 1
            weightsMatrix[v1, v2] = w
            weightsMatrix[v2, v1] = w
		end
	end
	return subgrafoInduzidoData(numberOfVertices, adjMatrix, weightsMatrix)
end

model = Model(HiGHS.Optimizer)

file = open(ARGS[1], "r")

data = readData(file)

# Vértice i pertence ao conjunto S
@variable(model, S[i = 1:data.numberOfVertices], Bin)

# Aresta ij está no subgrafo induzido
@variable(model, x[i = 1:data.numberOfVertices, j = 1:data.numberOfVertices], Bin)

# A restrição de comporta como uma porta lógica AND
# x[i, j] = S[i] and S[j]
for i in 1:data.numberOfVertices
  for j in 1:data.numberOfVertices
    if data.adjMatrix[i, j] == 1
      @constraint(model, x[i, j] >= S[i] + S[j] - 1)
      @constraint(model, x[i, j] <= S[i])
      @constraint(model, x[i, j] <= S[j])
    end
  end
end



# Minimizar o número de cores utilizadas@objective(model, Min, sum(S[i] * data.adjMatrix[i] for i in 1:data.numberOfVertices))
@objective(model, Max, sum(x[i, j] * data.weightsMatrix[i, j] for i in 1:data.numberOfVertices for j in 1:data.numberOfVertices))

print(model)

optimize!(model)

sol = objective_value(model)
println("Valor otimo = ", sol)

for i in 1:data.numberOfVertices
  if value(S[i]) >= 0.5
    print("$i ")
  end
end