import random
import string
import sys
import glob
import os
import time
import numpy as np

num_of_testcases = 0

def filename(test_name, node_num, edge_num):
  global num_of_testcases
  num_of_testcases = num_of_testcases + 1
  return "{}.{}.{}_{}".format(str(num_of_testcases).zfill(3), test_name, node_num, edge_num)

def remove_cases():
  cases = glob.glob('test/*')
  for c in cases:
    print("Removing " + c, file=sys.stderr)
    os.remove(c)

  cases = glob.glob('known_solutions/*')
  for c in cases:
    print("Removing " + c, file=sys.stderr)
    os.remove(c)

  cases = glob.glob('pseudotopological_solutions/*')
  for c in cases:
    print("Removing " + c, file=sys.stderr)
    os.remove(c)

  cases = glob.glob('obican_dfs_solutions/*')
  for c in cases:
    print("Removing " + c, file=sys.stderr)
    os.remove(c)

#theoretic solution: x == -1 -> unknownn, x == -2 -> same as sol, x >= 0 -> fixed
def testcase(test_name, node_num, edge_num, edge_list, theoretic_solution, exact_solution = ""):  
  if node_num <= 20:
    exact_solution = "algoritmi/brut"
  if exact_solution == "":
    exact_solution = "algoritmi/referentna_heuristika";
  fname = filename(test_name, node_num, edge_num)
  test_filename = "test/" + fname
  known_solutions_filename = "known_solutions/" + fname
  pseudotopological_solutions_filename = "pseudotopological_solutions/" + fname 
  obican_dfs_solutions_filename = "obican_dfs_solutions/" + fname
  print("Creating " + test_filename, file=sys.stderr)
  with open(test_filename, 'w') as f:
    f.write("{} {}\n".format(node_num, edge_num))
    for (x, y) in edge_list:
      f.write("{} {}\n".format(x, y))  
  
  print("Creating " + known_solutions_filename, file=sys.stderr)
  start = time.time()
  os.system('%s < %s > %s' % (exact_solution, test_filename, known_solutions_filename))  
  end = time.time()
  running_time = end - start
  running_time = '{0:.2f}'.format(running_time)
  os.system('echo "%s" >> %s' % (theoretic_solution, known_solutions_filename))  
  os.system('echo "%s" >> %s' % (running_time, known_solutions_filename))  

  print("Creating " + pseudotopological_solutions_filename, file=sys.stderr)
  start = time.time()
  os.system('%s < %s > %s' % ('LSP_pseudo_topological/lsp_unweighted', test_filename, pseudotopological_solutions_filename))  
  end = time.time()
  running_time = end - start
  running_time = '{0:.2f}'.format(running_time)
  os.system('echo "%s" >> %s' % (running_time, pseudotopological_solutions_filename))  
   
  print("Creating " + obican_dfs_solutions_filename, file=sys.stderr)
  start = time.time()
  os.system('./algoritmi/obican_dfs < %s > %s' % (test_filename, obican_dfs_solutions_filename))  
  end = time.time()
  running_time = end - start
  running_time = '{0:.2f}'.format(running_time)
  os.system('echo "%s" >> %s' % (running_time, obican_dfs_solutions_filename))  
  print()

def Hamiltonian(node_num, edge_num):
  edge_list = []
  edge_set = set()
  perm = np.random.permutation(node_num)
  for i in range(node_num - 1):
    edge_list.append((perm[i] + 1, perm[i + 1] + 1)) 
    edge_set.add((perm[i] + 1, perm[i + 1] + 1))
  for i in range(edge_num - node_num + 1):
    x = random.randint(1, node_num)
    y = random.randint(1, node_num)
    while x == y or ((x, y) in edge_set) or ((y, x) in edge_set):
      x = random.randint(1, node_num)
      y = random.randint(1, node_num)
    edge_list.append((x, y))
    edge_set.add((x, y))
  edge_list = np.random.permutation(edge_list)
  testcase('Hamiltonian', node_num, edge_num, edge_list, node_num)  

def Tree(node_num, edge_num):
  assert(edge_num == node_num - 1)
  edge_list = [] 
  perm = np.random.permutation(node_num)
  for i in range(2, node_num + 1):
    x = random.randint(1, i - 1)
    edge_list.append((perm[x - 1] + 1, perm[i - 1] + 1))
  testcase('Tree', node_num, edge_num, edge_list, -2, "algoritmi/stablo")  

def Generic(node_num, edge_num):
  assert(node_num <= edge_num)
  edge_list = [] 
  edge_set = set()
  perm = np.random.permutation(node_num)
  for i in range(2, node_num + 1):
    x = random.randint(1, i - 1)
    edge_list.append((perm[x - 1] + 1, perm[i - 1] + 1))  
    edge_set.add((perm[x - 1] + 1, perm[i - 1] + 1)) 
  for i in range(edge_num - node_num + 1):
    x = random.randint(1, node_num)
    y = random.randint(1, node_num)
    while x == y or ((x, y) in edge_set) or ((y, x) in edge_set):
      x = random.randint(1, node_num)
      y = random.randint(1, node_num)
    edge_list.append((x, y))
    edge_set.add((x, y))
  testcase('Generic', node_num, edge_num, edge_list, -1)  

if __name__ == "__main__":
  remove_cases()
  random.seed(11102020) 

  Tree(15, 14)
  Tree(50, 49)
  Tree(1000, 999)
  Tree(10000, 9999)
  Hamiltonian(20, 40)
  Hamiltonian(15, 30)
  Hamiltonian(5000, 10000)
  Hamiltonian(10000, 11000)
  Hamiltonian(10000, 40000)
  Generic(10, 20)
  Generic(30, 50)
  Generic(500, 10000)
  Generic(5000, 10000)
  Generic(7000, 15000)
  Generic(10000, 11000)
  Generic(10000, 35000)
  Generic(10000, 50000)
