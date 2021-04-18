import sys
import os
import glob
import time
import pandas as pd
from datetime import datetime
import matplotlib.pyplot as plt
import visualizer

sys.setrecursionlimit(1000010) 

def validate_and_get_data(test_file, out_file, special_type = ""):
  test_lines = open(test_file).readlines()

  n, m = map(int, test_lines[0].split())
  
  assert(1 <= n)
  assert(1 <= m)
  assert(n - 1 <= m <= n * (n - 1) / 2)
  
  bridovi = set()

  for i in range(m):
    x, y = map(int, test_lines[i + 1].split())
    assert(1 <= x <= n)
    assert(1 <= y <= n)
    assert(x != y)
    assert(not (x, y) in bridovi)
    bridovi.add((x, y))
    bridovi.add((y, x))
  
  out_lines = open(out_file).readlines()
  if special_type == "pseudotopological": 
    sol = int(out_lines[-3])
    niz = out_lines[-2].split()
    assert(len(niz) == sol)
    for i in range(sol - 1):
      x = int(niz[i])
      y = int(niz[i + 1])
      assert((x, y) in bridovi) 
    rt = float(out_lines[-1])
    return (n, m, sol, rt)
  else: 
    sol = int(out_lines[0])
    niz = out_lines[1].split()
    assert(len(niz) == sol)
    for i in range(sol - 1):
      x = int(niz[i])
      y = int(niz[i + 1])
      assert((x, y) in bridovi) 
    if special_type == "":
      return (n, m, sol)
    elif special_type == "knownsol":
      t = int(out_lines[2])
      rt = float(out_lines[3])
      return (n, m, sol, t, rt)
    else:
      assert(False)


def iterations_validate_and_get_data(test_file, out_file, special_type = ""):
  test_lines = open(test_file).readlines()

  n, m = map(int, test_lines[0].split())
  
  assert(1 <= n)
  assert(1 <= m)
  assert(n - 1 <= m <= n * (n - 1) / 2)
  
  bridovi = set()

  for i in range(m):
    x, y = map(int, test_lines[i + 1].split())
    assert(1 <= x <= n)
    assert(1 <= y <= n)
    assert(x != y)
    assert(not (x, y) in bridovi)
    bridovi.add((x, y))
    bridovi.add((y, x))
  
  out_lines = open(out_file).readlines()
  sol = int(out_lines[0])
  niz = out_lines[1].split()
  assert(len(niz) == sol)
  for i in range(sol - 1):
    x = int(niz[i])
    y = int(niz[i + 1])
    assert((x, y) in bridovi) 
  generations = int(out_lines[2])
  per_generation = out_lines[3].split()
  assert(len(per_generation) == generations)
  return (n, m, sol, per_generation)

def napravi_grafiku(out_file, tip, n, m, sol, per_generation):
  
  plt.figure(figsize=(12, 9))
  plt.plot(list(range(len(per_generation))), per_generation, color='blue')
  
  num_of_iterations = len(per_generation)
  ticks = []
  for i in range(5, num_of_iterations + 1, 5):
    ticks.append(i)
  plt.xticks(ticks, fontsize=15) 
  plt.yticks(fontsize=15)
  
  plt.xlabel("broj generacija", fontsize=15)
  plt.ylabel("najduži put", fontsize=15)  
  plt.suptitle('vrsta primjera: ' + tip, fontsize=20)
  plt.title('broj čvorova: {}, broj bridova: {}'.format(n, m), fontsize=16)
  plt.savefig(out_file + '.png')
  #plt.show()

#df = pd.DataFrame({'algo_name':['Tensor 2D', 'Tensor 3D: age', 'Tensor 4D: gender, age'], 'RMSE': [0.87, 1.2, 1.15]}) 
#plot_bar(df, 'Usporedba minimumi', 10000)
def plot_bar(bar_name, df, plot_name, title): 
  df.plot.bar(figsize=(10, 10), x='algo_name', y='najduzi_put', title = 'ajmoooo', legend = False, rot=0, color = ['blue', 'orange', 'green', 'yellow', 'red', 'purple'])
  plt.xticks(fontsize=15) 
  plt.yticks(fontsize=15)
  plt.xlabel('')
  plt.ylabel('najduži put', fontsize=15) 
  plt.ylim(bottom=0.6)  
  plt.suptitle(plot_name, fontsize=25)
  plt.title(title, fontsize=20)
  plt.savefig(bar_name + '.png')
  #plt.show()

def stvori_ili_obrisi(ime):
  if not os.path.exists(ime):
    os.makedirs(ime)
  else: 
    cases = glob.glob(ime + '/*')
    for c in cases:
      print("Removing " + c, file=sys.stderr)
      os.remove(c)

def sredi_direktorije():
  stvori_ili_obrisi('prezentacija_generacije_graficki')
  stvori_ili_obrisi('prezentacija_usporedba_rjesenja')
  stvori_ili_obrisi('prezentacija_gifovi')

if __name__ == "__main__":
  argv = sys.argv
  assert(len(argv) == 2)
  file_path = argv[1]
  assert(os.path.exists(file_path))
  assert(os.access(file_path, os.X_OK))

  assert(len(file_path) > 10)
  assert(file_path[0:10] == 'algoritmi/')
  sredi_direktorije()
  cases = glob.glob('test/*')
  cases.sort()
  prvi = True
  for test_file in cases: 
    print("Running on: " + test_file + "...", file=sys.stderr)
    out_file = 'pomocni'
    plot_name = 'prezentacija_generacije_graficki/' + test_file[5:]
    bar_name = 'prezentacija_usporedba_rjesenja/' + test_file[5:]
    gif_name = 'prezentacija_gifovi/' + test_file[5:]    
    start = time.time()
    os.system('%s < %s > %s' % (file_path, test_file, out_file))  
    end = time.time()
    running_time = end - start
    running_time = '{0:.2f}'.format(running_time)
    knownsol_file = test_file.replace("test", "known_solutions");
    n, m, sol, mess, rt = validate_and_get_data(test_file, knownsol_file, "knownsol")
    assert(mess >= -2)
    if mess == -1:
      opt = "?"
    elif mess == -2:
      opt = sol
    else:
      opt = mess

    print('num of nodes: {}, num of edges: {}, known optimum: {}'.format(n, m, opt))
    print('referentna heuristika: {}, running time: {}'.format(sol, rt))
    ref_sol = sol

    obican_dfs_file = test_file.replace("test", "obican_dfs_solutions");
    n, m, sol = validate_and_get_data(test_file, obican_dfs_file) 
    print('obican_dfs: {}, running time: {}'.format(sol, rt))
    obican_dfs_sol = sol

    pseudo_file = test_file.replace("test", "pseudotopological_solutions");
    n, m, sol, rt = validate_and_get_data(test_file, pseudo_file, "pseudotopological") 
    print('pseudotopological: {}, running time: {}'.format(sol, rt))
    pseudo_sol = sol
    
    tip = test_file.split('.')[1]
    n, m, sol, per_generation = iterations_validate_and_get_data(test_file, out_file)
    print('tip: {}, n: {}, n: {}, sol: {}, generations: {}\n'.format(tip, n, m, sol, per_generation))
    napravi_grafiku(plot_name, tip, n, m, sol, per_generation)
    moj_sol = sol

    putevi = [obican_dfs_sol, pseudo_sol, moj_sol]
    df = pd.DataFrame({'algo_name': ['običan DFS', 'rješenje iz članka', 'genetski'], 'najduzi_put': putevi}) 
    print("plotam.. ", tip)
    plot_bar(bar_name, df, 'vrsta primjera: ' + tip, 'broj čvorova: {}, broj bridova: {}'.format(n, m))

    if n <= 50 and m <= 50:
      G = visualizer.vrati_networkx_graph(test_file)
      put = visualizer.vrati_put(out_file)
      visualizer.animate_nodes(gif_name, G, put)
