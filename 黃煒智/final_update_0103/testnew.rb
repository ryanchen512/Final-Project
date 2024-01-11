5 7
1 2 10 5
1 3 20 8
2 3 15 6
2 4 25 10
3 4 30 12
3 5 15 6
4 5 20 8
insert 1 2 { 1 2 3 4 5 } 15
tree id = 1 root = 2 bandwidth cost = 28
{ 2 3 }{ 3 5 }{ 1 3 }{ 4 5 }
insert 2 3 { 1 2 3 4 5 } 15
tree id = 2 root = 3 bandwidth cost = 22
{ 3 4 }{ 2 4 }
insert 3 4 { 1 2 3 4 5 } 15
tree id = 3 root = 4 bandwidth cost = 12
{ 3 4 }
stop 1
tree id = 2 root = 3 bandwidth cost = 36
{ 3 4 }{ 2 4 }{ 3 5 }{ 1 3 }
tree id = 3 root = 4 bandwidth cost = 26
{ 3 4 }{ 2 3 }{ 4 5 }
stop 2
tree id = 3 root = 4 bandwidth cost = 34
{ 3 4 }{ 2 3 }{ 4 5 }{ 1 3 }
insert 4 1 { 1 2 3 4 5 } 5
tree id = 4 root = 1 bandwidth cost = 27
{ 1 2 }{ 1 3 }{ 3 5 }{ 4 5 }