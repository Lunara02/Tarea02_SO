# Tarea02_SO

FABRICA:
    Para poder acceder a al algoritmo de cola debe meterse a la carpeta llamada "simulador_cola" posteriormente poder ejecutar el archivo
    para ejecutar debe hacer primero:
        g++ cola.c -o simulapc
    despues para ejecutarlo debe usar
        ./simulapc -p x -c y -s z -t w
    
    tabla de significados:
       -p = Productores
        x = Cantidad Productores
       -c = Consumidores
        y = Cantidad Consumidores
       -s = Cola
        z = Tamaño de la cola
       -t = Espera
        w = Tiempo de espera    
    
    Cabe destacar que cada numero sea x, y, z o w tiene que estar junto a su entradad correspondiente ejemplos:

    Correcto:
        ./a -p 10 -c 5 -s 50 -t 10

    Incorrecto:
        ./ -c -p 10 4 -t 3 2 -s

GESTION DE MEMORIA:
    Para poder acceder a al algoritmo de gestion virtual debe meterse a la carpeta llamada "memoria_virtual" posteriormente poder ejecutar el archivo
    para ejecutar debe hacer primero:
        g++ memVirtual.cpp algoritmos.cpp -o mvirtual
    despues para ejecutarlo debe usar
        ./mvirtual x alg

    Tabla de significados:
          x = cantidad de marcos iniciales
        alg = nombre del algoritmo de reemplazo
referencias.txt = nombre de archivo con referencias a memoria dadas en número de página virtual
 

