import numpy as np
import matplotlib.pyplot as plt; 
import networkx as nx
from matplotlib.animation import FuncAnimation
import sys
import os

#0-indeksira 1-indeksirani input!
def vrati_networkx_graph(test_file):
  test_lines = open(test_file).readlines()
  n, m = map(int, test_lines[0].split())
  assert(1 <= n)
  assert(1 <= m)
  assert(n - 1 <= m <= n * (n - 1) / 2)
  bridovi = set()
  G = nx.Graph()
  G.add_nodes_from(range(n))
  for i in range(m):
    x, y = map(int, test_lines[i + 1].split())
    assert(1 <= x <= n)
    assert(1 <= y <= n)
    assert(x != y)
    assert(not (x, y) in bridovi)
    bridovi.add((x, y))
    bridovi.add((y, x))
    G.add_edge(x - 1, y - 1)   
  return G

#0-indeksira 1-indeksirani input!
def vrati_put(out_file):
  out_lines = open(out_file).readlines()
  sol = int(out_lines[0])
  niz = out_lines[1].split()
  for i in range(len(niz)):
    niz[i] = int(niz[i]) - 1
  return niz

def animate_nodes(gif_name, G, path, pos=None, *args, **kwargs):

  plt.close('all') #moozda ne treba
  
  # define graph layout if None given
  if pos is None:
    pos = nx.spring_layout(G)

  # draw graph

  edges = nx.draw_networkx_edges(G, pos,  *args, **kwargs)
  nodes = nx.draw_networkx_nodes(G, pos, node_size = [50] * len(G),  *args, **kwargs)

  boje = [0 for i in G.nodes]
  boje = np.array(boje)

  segments = edges.get_segments()

  bridovi = ['g' for edge in segments]
  

  plt.axis('off')


  #pretpostavljamo da su cvorovi 0-indeksirani
  def get_edge_index(a, b):
    for i in range(len(segments)):
      if (np.array([pos[a], pos[b]]) == segments[i]).all():
        return i
      if (np.array([pos[b], pos[a]]) == segments[i]).all():
        return i
    assert(False)

  def update(ii):
    # nodes are just markers returned by plt.scatter;
    # node color can hence be changed in the same way like marker colors
    boje[path[ii]] = 1
    nodes.set_array(boje)
    if ii > 0:
      bridovi[get_edge_index(path[ii - 1], path[ii])] = 'r'
    edges.set_color(bridovi)
    return edges, nodes

  fig = plt.gcf()
  animation = FuncAnimation(fig, update, interval=10000/len(path), frames=len(path), blit=True) 
  #plt.show()
  animation.save(gif_name + '.gif', writer='imagemagick', savefig_kwargs={'facecolor':'white'}, fps = int(len(path) / 10) + 1)
  #os.system("xdg-open animation.gif")
  #return animation

def dfs(graph, x, path):
  for y in graph.neighbors(x):
    if not y in path:
      path.append(y)
      dfs(graph, y, path)
      return

if __name__ == "__main__":
  
  if len(sys.argv) == 3:
    argv = sys.argv
    graph_data = argv[1]
    path_data = argv[2]
    assert(os.path.exists(graph_data))
    assert(os.path.exists(path_data))
    animate_nodes(vrati_networkx_graph(graph_data), vrati_put(path_data))
  else:
    print("testni.......")
    total_nodes = 30
    graph = nx.complete_graph(total_nodes)
    path = [0]
    dfs(graph, 0, path)
    animate_nodes(graph, path)
