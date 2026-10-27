using System.Runtime.InteropServices;
using System.Text.Json;

// The build
// [c file and cpp file] -> llvm-ir -> [dll/so and exe]
// We aren't use lib file, because some fucking bug from the C abi
// the llvm-ir is like that lib file

namespace make_llvm_ir
{
    class Build
    {
        // env info
        static string OS_NAME = "";
        static string OUTPUT_CPU_PLATFORM = "";
        static string DLL_EXTENSION = "";
        static string EXE_EXTENSION = "";

        static string PROJECT_ADDITIONAL_CMD = "";

        // source code info
        static ProjectInfo source_info = new ProjectInfo();

        private static bool InitCompiler()
        {
            if (OS_NAME == "")
            {
                if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
                {
                    DLL_EXTENSION = "{0}.dll";
                    EXE_EXTENSION = "{0}.exe";
                    OS_NAME = "WINDOWS";
                }
                else if (RuntimeInformation.IsOSPlatform(OSPlatform.Linux))
                {
                    DLL_EXTENSION = "lib{0}.so";
                    EXE_EXTENSION = "{0}";
                    OS_NAME = "LINUX";
                }
                else
                {
                    Console.WriteLine("NO SUPPORT THAT OS {0}", OS_NAME);
                    return false;
                }
            }
            OUTPUT_CPU_PLATFORM = RuntimeInformation.ProcessArchitecture.ToString();
            if (RuntimeInformation.ProcessArchitecture == Architecture.X64)
                PROJECT_ADDITIONAL_CMD += " -mavx -mavx2";
            return true;
        }
        private static bool BuildProject(string out_type)
        {
            Console.WriteLine("-------build-------");
            Console.WriteLine("project name\t:{0}", source_info.GetProjectName());
            Console.WriteLine("use clang make project");
            bool jump_link = false;

            string included = source_info.LoadIncluded();
            string defined = source_info.LoadDefined(OUTPUT_CPU_PLATFORM, OS_NAME);

            string PROJECT_OUT_PATH = source_info.GetProjectOutPath();
            if (PROJECT_OUT_PATH == null || PROJECT_OUT_PATH == "")
            {
                if (!File.Exists(PROJECT_OUT_PATH))
                {
                    Console.WriteLine("NO OUTPUT PATH");
                    return false;
                }
            }
            
            Console.WriteLine($"output directory: {PROJECT_OUT_PATH}");
            string out_file = "";
            if (out_type == "EXE")
                out_file = string.Format(EXE_EXTENSION, source_info.GetProjectName());
            else if (out_type == "DLL")
                out_file = string.Format(DLL_EXTENSION, source_info.GetProjectName());
            else if (out_type == "LIB" || out_type == "IR")
                jump_link = true;
            else
            {
                Console.WriteLine("NO OUT FILE TYPE");
                return false;
            }
            List<string> source_list = source_info.GetSourceList();
            int src_num = source_list.Count + 1;
            Console.WriteLine($"The {src_num} files will be compiled");
            Console.WriteLine("-------compiling stsrt-------");
            for (int i = 0; i < source_list.Count; ++i)
            {
                CompilerThread ct = new CompilerThread(i + 1, source_list[i], included, defined, PROJECT_OUT_PATH);
                ct.DoCompile(PROJECT_ADDITIONAL_CMD, source_info.GetOptimizationLevel(), false);
            }
            Console.WriteLine("-------compiling finish-------");
            return true;
        }
        private static int Main(string[] args)
        {   
            if (!InitCompiler()) { return 2; }
            if (!source_info.ReadJson(args[0])) { return 3; }
            if (!BuildProject(args[1])) { return 4; }
            return 0;
        }
    }
}