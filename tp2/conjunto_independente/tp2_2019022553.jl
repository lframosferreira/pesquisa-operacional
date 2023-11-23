mutable struct conjuntoIndependenteData
  numberOfVertices::Int64 # número de vértices
  adjList::Array{Array{Int64}} # Matriz de adjacências
  degrees::Array{Int64} # Vetor com graus dos vértices
end

function readData(file)
  numberOfVertices = 0
  adjList = missing
  degrees = missing
  for line in eachline(file)
    q = split(line, "\t")
    if (q[1] == "n")
      numberOfVertices = parse(Int64, q[2])
      adjList = [[] for i = 1:numberOfVertices]
      degrees = zeros(Int64, numberOfVertices)
    elseif q[1] == "e"
      v1 = parse(Int64, q[2])
      v2 = parse(Int64, q[3])
      degrees[v1] += 1
      degrees[v2] += 1
      push!(adjList[v1], v2)
      push!(adjList[v2], v1)
    end
  end
  return conjuntoIndependenteData(numberOfVertices, adjList, degrees)
end

file = open(ARGS[1], "r")

data = readData(file)

sorted_indices = sortperm(data.degrees)
sort!(data.degrees)

solution = zeros(Int64, data.numberOfVertices)

# Estou ordenando pelos graus iniciais. Possivelemnte seria melhor ordenar a cada iteração

function solve()
  covered = zeros(Int64, data.numberOfVertices) # Mantêm controle sobre vértices já cobertos
  count = 0
  vertices_sol = []
  for i in 1:data.numberOfVertices
    idx = sorted_indices[i]
    if (covered[idx] == 0)
      covered[idx] = 1
      solution[idx] = 1
      count += 1
      push!(vertices_sol, idx)
      for j in data.adjList[idx]
        covered[j] = 1
      end
    end
  end
  
  sort!(vertices_sol)
  println("TP2 2019022553 = ", count)
  for i in 1:length(vertices_sol)
    if i == length(vertices_sol)
      print("$(vertices_sol[i])")
    else
      print("$(vertices_sol[i])\t") 
    end
  end
  println()
end

solve()
