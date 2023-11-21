mutable struct lotSizingData
  numberOfPeriods::Int64 # número de períodos
  productionCost::Array{Int64} # vetor de custos de produção por período
  demand::Array{Int64} # vetor de demanda por período
  storageCost::Array{Int64} # vetor de custos de estocagem por período
  assessmentCost::Array{Int64} # vetor de custos das multas por período
end

function readData(file)
  numberOfPeriods = missing
  productionCost = missing
  demand = missing
  storageCost = missing
  assessmentCost = missing
  for line in eachline(file)
    q = split(line, "\t")
    if q[1] == "n"
      numberOfPeriods = parse(Int64, q[2])
      productionCost = zeros(Float64, numberOfPeriods)
      demand = zeros(Float64, numberOfPeriods)
      storageCost = zeros(Float64, numberOfPeriods)
      assessmentCost = zeros(Float64, numberOfPeriods)
    elseif q[1] == "c"
      productionCost[parse(Int64, q[2])] = parse(Float64, q[3])
    elseif q[1] == "d"
      demand[parse(Int64, q[2])] = parse(Float64, q[3])
    elseif q[1] == "s"
      storageCost[parse(Int64, q[2])] = parse(Float64, q[3])
    elseif q[1] == "p"
      assessmentCost[parse(Int64, q[2])] = parse(Float64, q[3])
    end
  end
  return lotSizingData(numberOfPeriods, productionCost, demand, storageCost, assessmentCost)
end

file = open(ARGS[1], "r")

data = readData(file)


