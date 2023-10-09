# Script basico para juntar a dados dos resultados

import sys
import statistics

i = 0
current_test  = ""
test          = []
result        = []
total_time    = []
partial_time  = []

# Abre o arquivo e percorre linha a linha
with open(sys.argv[1],"r") as file:
    for line in file:
        # nome e iteração do teste
        if i == 0 :
            if current_test != line.split()[1] :
                current_test  = line.split()[1]
                test.append(current_test)
                flag_new_test = True
        # Verifica se o resultado se manteve nas iteracoes
        if i == 1 :
            if flag_new_test : 
                validator = line
                result.append(int(line.strip("\n")))
                flag_new_test = False
            else :
                if validator != line : 
                    print("# Error :: Iteration returned diferent result")
        # Tempo de execucao
        if i == 2 : 
            total_time.append(float(line.strip("\n")))
        # Tempo especifico (tempo passado em paralelo, ou se a função for sequencial o tempo gasto que não pode ser paralelizado)_
        if i == 3 : 
            partial_time.append(float(line.strip("\n")))
            i=-1
        i = i+1

try:
    analise = open("analise_"+sys.argv[1],"x")
    for t in test : 
        analise.write("\n# " + t + "\n")
        analise.write("Resposta      : " + str(result.pop(0)) + "\n")
        temp =[]
        for iter in range(20) : 
            temp.append(total_time.pop(0))
        analise.write("Tempo Total   : " + str(statistics.mean(temp)) + "   ("+str(statistics.stdev(temp)) + ")\n"  )

        temp =[]
        for iter in range(20) : 
            temp.append(partial_time.pop(0))
        analise.write("Tempo parcial : " + str(statistics.mean(temp)) + "   ("+str(statistics.stdev(temp)) + ")\n"  )
        analise.close
except:
    print("# Error :: File [" + "analise_"+sys.argv[1] + "] already exists!")