using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text;

namespace make_llvm_ir
{
    internal class CompilerThread
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

        public string DoCompile(ref int exit_code, string uni_additional_command, string optimization_level, bool enable_commandLine = false)
        {
            string output_file = Path.Join(m_output_file_path, (Path.GetFileNameWithoutExtension(m_sources_string) + ".ll"));
            string arg = $"{optimization_level} {m_includes_string}{m_marcos_string}{m_sources_string} -o {output_file}";
            string ifi = Path.GetFileName(m_sources_string);
            string ofi = Path.GetFileName(output_file);
            Console.WriteLine($"file {m_index}\t\tcompilting: {ifi} -> {ofi}");

            arg = $"-std=c++23 -emit-llvm -S {uni_additional_command} " + arg;
            if (enable_commandLine)
                Console.WriteLine($"clang++ {arg}");
            ProcessStartInfo psi = new ProcessStartInfo();
            
            psi.FileName = "D:/Program Files/Microsoft Visual Studio/18/Community/VC/Tools/Llvm/x64/bin/clang++.exe";
            psi.Arguments = arg;
            psi.UseShellExecute = false;
            psi.RedirectStandardOutput = true;
            psi.CreateNoWindow = true;
            using (Process p = Process.Start(psi))
            { 
                p.WaitForExit();
                exit_code = p.ExitCode;
                if (p.StandardOutput.ReadToEnd() != "")
                    Console.WriteLine(p.StandardOutput.ReadToEnd());
            }
            if (exit_code == 0)
                return output_file;
            else
                return "";
        }
    }
}
