import sys
import os
import glob
import time
from datetime import datetime

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

if __name__ == "__main__":
  argv = sys.argv
  assert(len(argv) == 2)
  file_path = argv[1]
  assert(os.path.exists(file_path))
  assert(os.access(file_path, os.X_OK))

  assert(len(file_path) > 10)
  assert(file_path[0:10] == 'algoritmi/')
  if not os.path.exists('submissions_log'):
    os.makedirs('submissions_log')
 
  source_path = file_path + '.cpp'
  assert(os.path.exists(source_path))

  dt_string = datetime.now().strftime("%Y_%m_%d_%H_%M_%S") 
  algorithm_name = file_path[10:]
  folder_name = 'submissions_log/' + algorithm_name + '_' + dt_string
  output_files = folder_name + '/output_files'
  if not os.path.exists(folder_name):
    os.makedirs(folder_name)
    os.makedirs(output_files)
  
  cases = glob.glob('test/*')
  cases.sort()
  prvi = True
  for test_file in cases: 
    print("Running on: " + test_file + "...", file=sys.stderr)
    out_file = output_files + '/' + test_file[5:]
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

    pseudo_file = test_file.replace("test", "pseudotopological_solutions");
    n, m, sol, rt = validate_and_get_data(test_file, pseudo_file, "pseudotopological") 
    print('pseudotopological: {}, running time: {}'.format(sol, rt))

    n, m, sol = validate_and_get_data(test_file, out_file)
    print('this solution: {}, running time: {}\n'.format(sol, running_time))
    if(prvi):
      prvi_test_file = test_file
      prvi_out_file = out_file
      prvi = False
  import visualizer
  G = visualizer.vrati_networkx_graph(prvi_test_file)
  put = visualizer.vrati_put(prvi_out_file)
  visualizer.animate_nodes('animation', G, put)
  os.system("rm %s -r" % folder_name)
