using System.Diagnostics;

namespace make_llvm_ir
{
    internal class LinkThread
    {
        public int Link(string uni_additional_command, string libs, string additional_command, string optimization_level, string linked_file_cmd, string out_file_Path, bool enable_commandLine = false)
        {
            ProcessStartInfo psi = new ProcessStartInfo();
            int exit_code=0;
            string arg = $"{uni_additional_command} -fuse-ld=lld -flto=full -fvisibility=hidden {additional_command} {optimization_level} {linked_file_cmd} {libs}-o {out_file_Path}";
            if (enable_commandLine)
                Console.WriteLine($"clang++ {arg}");

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
            
            return exit_code;
        }
    }
}