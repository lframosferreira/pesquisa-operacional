using JuMP
using HiGHS

mutable struct subgrafoInduzidoData
  numberOfVertices::Int64 # Número de vértices
  adjMatrix::Matrix{Int64} # Matriz de adjacências
  weightsMatrix::Matrix{Float64}
end

function readData(file)
  numberOfVertices = 0
  adjMatrix = missing
  weightsMatrix = missing
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
      weightsMatrix[v1, v2] = w
    end
  end
  return subgrafoInduzidoData(numberOfVertices, adjMatrix, weightsMatrix)
end

model = Model(HiGHS.Optimizer)

file = open(ARGS[1], "r")

data = readData(file)

# Si é igual a 1 se e somente se o vértice i pertence ao conjunto S
@variable(model, S[i=1:data.numberOfVertices], Bin)

# Xij é igual a 1 se e somente se aresta ij está no subgrafo induzido
@variable(model, x[i=1:data.numberOfVertices, j=1:data.numberOfVertices], Bin)

# A restrição de comporta como uma porta lógica AND
# x[i, j] = S[i] and S[j]
for i in 1:data.numberOfVertices
  for j in i:data.numberOfVertices
    if data.adjMatrix[i, j] == 1
      @constraint(model, x[i, j] >= S[i] + S[j] - 1)
      @constraint(model, x[i, j] <= S[i])
      @constraint(model, x[i, j] <= S[j])
    end
  end
end

# Minimizar o número de cores utilizadas
@objective(model, Max, sum(x[i, j] * data.weightsMatrix[i, j] for i in 1:data.numberOfVertices for j in 1:data.numberOfVertices))

print(model)

optimize!(model)

sol = objective_value(model)

println("TP1 2019022553 = ", sol)
