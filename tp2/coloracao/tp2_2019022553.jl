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

file = open(ARGS[1], "r")

data = readData(file)


function solve()
  
  solution = zeros(Int64, data.numberOfVertices)
  solution[1] = 1
  for i in 2:data.numberOfVertices
     
  end

end
