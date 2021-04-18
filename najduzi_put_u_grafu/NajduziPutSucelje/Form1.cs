using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Threading;
using System.IO;

namespace NajduziPutSucelje
{
    public partial class Form1 : Form
    {

        [DllImport("NPUGdll.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void runMain(string fileName);

        [DllImport("NPUGdll.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void getMessage(StringBuilder str, int len);
        //https://stackoverflow.com/questions/20752001/passing-strings-from-c-sharp-to-c-dll-and-back-minimal-example

        [DllImport("NPUGdll.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern int getGenerationNumber();

        [DllImport("NPUGdll.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern int getMaxGenerations();

        [DllImport("NPUGdll.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void reset();


        static StringBuilder sb;
        static int maxGeneration;

        System.Windows.Forms.Timer clock;
        Thread radnaDretva;

        public Form1()
        {
            InitializeComponent();

            //100000 max velicina poruke koja se ispisuje u sucelju
            sb = new StringBuilder(100000);

            maxGeneration = getMaxGenerations();
            progressBar.Visible = false;

            clock = new System.Windows.Forms.Timer();
            clock.Interval = 500;
            clock.Tick += new EventHandler(azurirajStanje);
        }

        private void testFileButton_Click(object sender, EventArgs e)
        {
            var fileDialog = new OpenFileDialog();
            //fileDialog.InitialDirectory = @"..\sucelje_testovi";
            if (fileDialog.ShowDialog() == DialogResult.OK)
            {
                //string fileToOpen = fileDialog.FileName;

                urlTextBox.Text = fileDialog.FileName;
            }
        }

        private void clearButton_Click(object sender, EventArgs e)
        {
            urlTextBox.Text = "";
        }

        private void runButton_Click(object sender, EventArgs e)
        {
            //Zabranit cemo pozivanje neodabranih datoteka i pozivanje procesa na novom test primjeru dok se jos izvrsava na starom.
            if(urlTextBox.Text != "" && !progressBar.Visible)
            {
                reset();

                ispisTextBox.Text = "TEST: " + urlTextBox.Text.Substring(urlTextBox.Text.LastIndexOf('\\') + 1) + "\r\n";
                int index1 = urlTextBox.Text.LastIndexOf('.');
                int index2 = urlTextBox.Text.LastIndexOf('_');
                string nodes = urlTextBox.Text.Substring(index1 + 1, index2 - index1 - 1);
                string edges = urlTextBox.Text.Substring(index2 + 1);
                ispisTextBox.Text += "Number of nodes: " + nodes + Environment.NewLine;
                ispisTextBox.Text += "Number of edges: " + edges + Environment.NewLine;
                ispisTextBox.Text += "Running..." + Environment.NewLine;

                radnaDretva = new Thread( () => runMain(urlTextBox.Text) );
                radnaDretva.Start();

                progressBar.Visible = true;
                progressBar.Value = 0;
                progressBar.Minimum = 0;
                progressBar.Maximum = 100;

                clock.Start();
            }
        }

        public void azurirajStanje(object sender, EventArgs e)
        {
            sb.Clear();
            getMessage(sb, sb.Capacity);
            ispisTextBox.Text += sb.ToString();

            int progress = (int)( ((double)getGenerationNumber() / maxGeneration) * 100);
            //Gotovo izvodenje
            //progress == 100 - obavljene sve iteracije, no zelimo i zavrsni dio odraditi nakon svih generacija.
            //Tada cemo postavit generaciju na jedan broj veci od maksimalnog broja generacija, te ce nam to biti signal da je 
            //gotov rad.
            if(progress > 100)
            {
                clock.Stop();
                radnaDretva.Join();
                progressBar.Visible = false;


                // Kad sve završi ispisujemo rezultate referentne heuristike i pseudotoploskog rjesenja
                // u fileName spremamo ime datoteke, npr. 003.Generic.5000_10000
                // Zatim preko StreamReadera citamo datoteko liniju po liniju i ispisujemo trazene rezultate
                string fileName = urlTextBox.Text.Substring(urlTextBox.Text.LastIndexOf('\\') + 1);

                System.IO.StreamReader file1 = new System.IO.StreamReader(@"..\..\..\obican_dfs_solutions\" + fileName);
                int counter = 0;
                string line, prev_line = "";
                string rezultat = "";
                while ((line = file1.ReadLine()) != null)
                {
                    if (counter == 0)
                    {
                        rezultat = line;
                    }
                    counter++;
                    prev_line = line;
                }
                ispisTextBox.Text += "Obican DFS: " + rezultat + ", running time: " + 
                                    prev_line + " s" + Environment.NewLine;
                file1.Close();
                
                // Isto to za pseudotopolosko rjesenje
                System.IO.StreamReader file2 = new System.IO.StreamReader(@"..\..\..\pseudotopological_solutions\" + fileName);
                bool flag = false;
                while ((line = file2.ReadLine()) != null)
                {
                    if (line[0] == 'P')
                    {
                        prev_line = line;
                        continue;
                    }
                    else if (flag == false)
                    {
                        rezultat = line;
                        flag = true;
                    }
                    prev_line = line;
                }
                ispisTextBox.Text += "Pseudotopological: " + rezultat + ", running time: " + 
                                     prev_line + " s" + Environment.NewLine;
                file2.Close();
            }
            else
            {
                progressBar.Value = progress;
            }
        }
    }
}
