import random

def generate_problem1_test_case(file_name):
    V = 99  
    E = V*(V-1)//2 
    notinsert=0
    anyinsert=False
    stops = []

    edges = []  # 保存边的列表
    edgestest = []

    for _ in range(E):
        u = random.randint(1, V)
        v = random.randint(1, V)
        while u == v or (u, v) in edgestest or (v, u) in edgestest:
            u = random.randint(1, V)
            v = random.randint(1, V)
        b = random.randint(1, 100)
        c = random.randint(1, 100)
        if u>v:
            t=u
            u=v
            v=t
        edgestest.append((u,v))
        edges.append((u, v, b, c))

    # 按照 u 由小到大的顺序对边进行排序
    edges.sort(key=lambda x: (x[0], x[1]))

    with open(file_name, 'w') as file:
        file.write(f"{V} {E}\n")
        for edge in edges:
            file.write(f"{edge[0]} {edge[1]} {edge[2]} {edge[3]}\n")
        
        file.write(f"\n")
        for i in range(1, 50000):
            select=random.randint(1, 7)
            if(select==1 or select==2 or select==3 or select==4):
                s=random.randint(1, 20)
                t=random.randint(1, 20)
                file.write(f"insert {i-notinsert} {s} ")
                file.write("{ ")
                for j in range(1, V+1):
                    file.write(f"{j} ")
                file.write("} ")
                file.write(f"{t}\n")
                anyinsert=True
            elif((select==5 or select==6)and anyinsert):
                notinsert+=1
                if(len(stops)==i-notinsert):
                    continue
                st=random.randint(1, i-notinsert)
                while(st in stops):
                    st=random.randint(1, i-notinsert)
                file.write(f"stop {st}\n")
                stops.append(st)
            elif(select==7 and anyinsert):
                file.write("rearrange\n")
                notinsert+=1


def generate_problem2_test_case(file_name):
    V = 99  
    E = V*(V-1)//2 
    notinsert=0
    anyinsert=False
    stops = []

    edges = []  # 保存边的列表
    edgestest = []

    for _ in range(E):
        u = random.randint(1, V)
        v = random.randint(1, V)
        while u == v or (u, v) in edgestest or (v, u) in edgestest:
            u = random.randint(1, V)
            v = random.randint(1, V)
        b = random.randint(1, 100)
        c = random.randint(1, 100)
        if u>v:
            t=u
            u=v
            v=t
        edgestest.append((u,v))
        edges.append((u, v, b, c))

    # 按照 u 由小到大的顺序对边进行排序
    edges.sort(key=lambda x: (x[0], x[1]))

    with open(file_name, 'w') as file:
        file.write(f"{V} {E}\n")
        for edge in edges:
            file.write(f"{edge[0]} {edge[1]} {edge[2]} {edge[3]}\n")
        
        file.write(f"\n")
        for i in range(1, 50000):
            select=random.randint(1, 7)
            if(select==1 or select==2 or select==3 or select==4):
                s=random.randint(1, 20)
                t=random.randint(1, 20)
                file.write(f"insert {i-notinsert} {s} ")
                file.write("{ ")
                max=random.randint(1, V+1)
                min=random.randint(1, V+1)
                if(min>max):
                    t=min
                    min=max
                    max=t
                for j in range(min, max):
                    file.write(f"{j} ")
                file.write("} ")
                file.write(f"{t}\n")
                anyinsert=True
            elif((select==5 or select==6)and anyinsert):
                notinsert+=1
                if(len(stops)==i-notinsert):
                    continue
                st=random.randint(1, i-notinsert)
                while(st in stops):
                    st=random.randint(1, i-notinsert)
                file.write(f"stop {st}\n")
                stops.append(st)
            elif(select==7 and anyinsert):
                file.write("rearrange\n")
                notinsert+=1

# 生成 Problem1 的测试数据
generate_problem1_test_case('Problem1_test_case.txt')
print("Problem1_test_case.txt generated.")

# 生成 Problem2 的测试数据
generate_problem2_test_case('Problem2_test_case.txt')
print("Problem2_test_case.txt generated.")