using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text;

namespace make_llvm_ir
{
    public class CompilerThread
    {
        private string m_sources_string;
        private string m_includes_string;
        private string m_marcos_string;
        private string m_output_file_path;
        private int m_index;

        public CompilerThread(int index, string sources_string, string includes_string, string marcos_string,
            string output_file_path)
        {
            m_sources_string = sources_string;
            m_includes_string = includes_string;
            m_marcos_string = marcos_string;
            m_output_file_path = output_file_path;
            m_index = index;
        }

        public int DoCompile(string additional_command, string optimization_level, bool enable_commandLine = false)
        {
            int exit_code = 0;
            string output_file = Path.Join(m_output_file_path, (Path.GetFileNameWithoutExtension(m_sources_string) + ".ll"));
            string arg = $"-{optimization_level} {additional_command} {m_includes_string}{m_marcos_string}{m_sources_string} -o {output_file}";
            string ifi = Path.GetFileName(m_sources_string);
            string ofi = Path.GetFileName(output_file);
            Console.WriteLine($"file {m_index}\t\tcompilting: {ifi} -> {ofi}");
            if (enable_commandLine)
                Console.WriteLine($"clang++ -std=c++23 -emit-llvm -S {arg}");
            ProcessStartInfo psi = new ProcessStartInfo();
            
            psi.FileName = "D:/Program Files/Microsoft Visual Studio/18/Community/VC/Tools/Llvm/x64/bin/clang++.exe";
            psi.Arguments = "-std=c++23 -emit-llvm -S " + arg;
            psi.UseShellExecute = false;
            psi.RedirectStandardOutput = false;
            psi.CreateNoWindow = true;
            using (Process p = Process.Start(psi))
            { 
                p.WaitForExit();
                exit_code = p.ExitCode;
            }
            
            return exit_code;
        }
    }
}
