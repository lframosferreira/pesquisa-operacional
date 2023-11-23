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
      weights[id+1] = weight
    end
  end
  return empacotamentoData(numberOfObjects, weights)
end

file = open(ARGS[1], "r")
data = readData(file)

#= Pensar nessa otimização. Acho que ela seria boa, mas cuidado com índices será necessário =#
sorted_indices = sortperm(data.weights)
sort!(data.weights)

solution = fill(-1, data.numberOfObjects, data.numberOfObjects)

function solve()
  idx = 1
  box = 1
  allowed_weight = 20.0
  for i in 1:data.numberOfObjects
    if (allowed_weight - data.weights[i] >= 0.0)
      solution[box, idx] = sorted_indices[i]
      allowed_weight -= data.weights[i]
      idx += 1
    else #= Caio aqui apenas quando não cabe um objeto na caixa atual, então o coloco na próxima e continuo =#
      idx = 1
      box += 1
      allowed_weight = 20.0 - data.weights[i]
      solution[box, idx] = sorted_indices[i]
      idx = 2
    end
  end


  #= Acabou a iteração, podemos printar a solução =#
  println("TP2 2019022553 = ", box - 1)
  for i in 1:box
    for j in 1:data.numberOfObjects
      if solution[i, j] == -1
        break
      end
      if j == data.numberOfObjects
        print("$(solution[i, j])")
      else
        print("$(solution[i, j])\t")
      end
    end
    println()
  end
end

solve()
