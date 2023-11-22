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

file = open(ARGS[1], "r")

data = readData(file
