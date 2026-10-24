using System.Runtime.InteropServices;

class Build
{
    static string OS_NAME = "";
    static string OUTPUT_CPU_PLATFORM = "";
    static string PROJECT_ROOT_PATH = "";
    static string COMPILER = "";
    static string OUT_DIR = "/out/app/";
    static string DLL_EXTENSION = "";
    static string LIB_EXTENSION = "";
    static string EXE_EXTENSION = "";

    static string IR_EXTENSION = "-o {0}.ll";

    static string IR_CMD = "";

    private static bool init_compiler()
    {
        if (OS_NAME == "")
        {
            if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
            {
                COMPILER = "clang-cl";
                DLL_EXTENSION = "-o {0}.dll";
                LIB_EXTENSION = "-o {0}.lib";
                EXE_EXTENSION = "-o {0}.exe";
                OS_NAME = "windows";
            }
            if (RuntimeInformation.IsOSPlatform(OSPlatform.Linux))
            {
                COMPILER = "clang";
                DLL_EXTENSION = "-o lib{0}.so";
                LIB_EXTENSION = "-o lib{0}.a";
                EXE_EXTENSION = "-o {0}";
                OS_NAME = "linux";
            }
            else
            {
                Console.WriteLine("NO SUPPORT THAT OS {0}", OS_NAME);
                return false;
            }
        }
        OUTPUT_CPU_PLATFORM = RuntimeInformation.ProcessArchitecture.ToString();
        return true;
    }

    private static string load_defined(List<string> marco_list)
    {
        string out_cmd = "";
        foreach (string marco in marco_list)
        {
            out_cmd = string.Format("-D{0} ", (marco));
        }
        return out_cmd;
    }
    private static string load_included(List<string> include_list)
    {
        string out_cmd = "";
        foreach (string include in include_list)
        {
            out_cmd = string.Format("-isystemF:\"{0}\" ", (include));
        }
        return out_cmd;
    }
    private static string load_source(List<string> source_list)
    {
        string out_cmd = "";
        foreach (string source in source_list)
        {
            out_cmd = string.Format("\"{0}\" ", (source));
        }
        return out_cmd;
    }

    private static string build(string out_type, string project_name, List<string> sources, List<string> include, Dictionary<string, Dictionary<string, List<string>>> marcos_dic)
    {
        string cmd = "";
        Console.WriteLine("-------build-------");
        Console.WriteLine("project name\t:{0}", project_name);

        string out_file = "";
        if (out_type == "EXE")
            out_file = string.Format(EXE_EXTENSION, project_name);
        if (out_type == "LIB")
            out_file = string.Format(LIB_EXTENSION, project_name);
        if (out_type == "DLL")
            out_file = string.Format(DLL_EXTENSION, project_name);
        if (out_type == "IR")
        {
            out_file = string.Format(IR_EXTENSION, project_name);
            IR_CMD = " -emit - llvm - S";
        }
        else
        {
            Console.WriteLine("NO OUT FILE TYPE");
            return "";
        }
        Console.WriteLine(out_file);

        List<string> macro_list = new List<string>();
        macro_list.AddRange((marcos_dic["os"])[OS_NAME]);
        macro_list.AddRange((marcos_dic["cpu"])[OUTPUT_CPU_PLATFORM]);
        cmd = cmd + load_defined(macro_list);

        cmd = cmd + load_included(include);
        cmd = cmd + load_source(sources);

        Console.WriteLine("-------compiling-------");

        cmd = string.Format("clang -Os{0} {1}", IR_CMD, cmd) + out_file;

        return cmd;
    }

    private static int Main(string[] args)
    {
        if (!init_compiler())
        {
            return -1;
        }
        string CORE_PROJECT_NAME = "wrcore";
        List<string> CORE_PROJECT_ADD_INCLUDE_DIR = [
            "/inc/",
            "/src/",
            "/3rd/inc/"
        ];
        List<string> CORE_PROJECT_SOURCE = [
            "/src/core/*cpp",
            "/inc/core/dynamic_array/*cpp",
            "/inc/core/string/*cpp",
            "/inc/os/*cpp",
            "/inc/os/linux/*cpp",
            "/inc/os/windows/*cpp",
            "/inc/os/dev/*cpp",
            "/inc/os/dev/arm-neon/*cpp",
            "/inc/os/dev/avx/*cpp",
        ];
        Dictionary<string, Dictionary<string, List<string>>> CORE_PROJECT_MARCO = new Dictionary<string, Dictionary<string, List<string>>>();
        build("IR", CORE_PROJECT_NAME, CORE_PROJECT_SOURCE, CORE_PROJECT_ADD_INCLUDE_DIR, CORE_PROJECT_MARCO);
        return 0;
    }
}