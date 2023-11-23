mutable struct coloracaoData
  numberOfVertices::Int64 # número de vértices
  adjList::Array{Array{Int64}} # Matriz de adjacências
  degrees::Array{Int64} # Graus dos vértices
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
      push!(adjList[v1], v2)
      push!(adjList[v2], v1)
      degrees[v1] += 1
      degrees[v2] += 1
    end
  end
  return coloracaoData(numberOfVertices, adjList, degrees)
end

file = open(ARGS[1], "r")

data = readData(file)

sorted_indices = sortperm(data.degrees, rev=true)
sort!(data.degrees)

function solve()
  colors = zeros(Int64, data.numberOfVertices) 
  solution = zeros(Int64, data.numberOfVertices)
  solution[sorted_indices[1]] = 1
  colors[1] = 1
  for i in sorted_indices[1:data.numberOfVertices]
    mask = zeros(Int64, data.numberOfVertices)
    for neighbour in data.adjList[i]
      if solution[neighbour] != 0
        mask[solution[neighbour]] = 1
      end
    end
    solution[i] = findfirst(isequal(0), mask)
  end

  vertex_per_color = [[] for i = 1:length(unique(solution))]
  for (idx, val) in enumerate(solution)
    push!(vertex_per_color[val], idx)
  end
  println("TP2 2019022553 = ", length(unique(solution)))
  for i in 1:length(unique(solution))
    for j in 1:length(vertex_per_color[i])
      if j == length(vertex_per_color[i])
        print("$(vertex_per_color[i][j])")
      else
        print("$(vertex_per_color[i][j])\t")
      end
    end
    println()
  end
end

solve()
