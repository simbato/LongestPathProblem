#include "digraph.hpp"
#include "digraph.hpp"
#include "path.hpp"
#include "trainer.hpp"
#include <cmath>

DiGraph ReadDigraphFromStdin()
{
	using std::cin;
	
	int n,m;
	cin >> n >> m;
	DiGraph D(n);
	for (int i = 0; i < m; ++i)
	{
		int a, b;
		cin >> a >> b;
    a--, b--;
    D.add_edge(a,b);
    D.add_edge(b,a);
	}
	return D;
}

int main() 
{
	using namespace std;
	std::ios_base::sync_with_stdio(false);
	
	auto D = ReadDigraphFromStdin();
	
	Path p = D.FindLongestSimplePath();
  //std::cout << "The best path I found has value " << p.value() << std::endl;
  //std::cout << "------------" << std::endl;
  std::cout << p.size() << std::endl;
  for(auto x : p.m_path)
    std::cout << x + 1 << " ";
  std::cout << std::endl;
    return 0;
}
