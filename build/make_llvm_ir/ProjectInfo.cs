using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Text;
using System.Text.Json;

namespace make_llvm_ir
{
    class ProjectJsonStruction
    {
        public string? PROJECT_NAME { get; set; }
        public string? PROJECT_OUTPUT_DIR { get; set; }
        public string? PROJECT_OPTIMIZATION_LEVEL { get; set; }
        public List<string>? PROJECT_SOURCE { get; set; }
        public List<string>? PROJECT_ADDITIONAL_INCLUDE_DIR { get; set; }
        public Dictionary<string, Dictionary<string, List<string>>>? PROJECT_ADDITIONAL_MARCO { get; set; }
        public Dictionary<string, List<string>>? PROJECT_ADDITIONAL_LIBS { get; set; }
    }
    class ProjectInfo
    {
        string PROJECT_ROOT_PATH = "";
        ProjectJsonStruction? json_info;
        public bool ReadJson(string json_path)
        {
            PROJECT_ROOT_PATH = Path.GetDirectoryName(json_path);
            if (!File.Exists(json_path))
            {
                Console.WriteLine("NO JSON FILE");
                return false;
            }
            string jsonString = File.ReadAllText(json_path);
            json_info = JsonSerializer.Deserialize<ProjectJsonStruction>(jsonString);
            if (json_info == null)
            {
                Console.WriteLine("NO TRUE STRUCTION JSON FILE");
                return false;
            }
            return true;
        }
        public string LoadDefined(string OUTPUT_CPU_PLATFORM, string OS_NAME)
        {
            List<string> macro_list = new List<string>();
            macro_list.AddRange((json_info.PROJECT_ADDITIONAL_MARCO["cpu"])[OUTPUT_CPU_PLATFORM]);
            macro_list.AddRange((json_info.PROJECT_ADDITIONAL_MARCO["os"])[OS_NAME]);
            string out_cmd = "";
            foreach (string marco in macro_list)
            {
                out_cmd += string.Format("-D{0} ", (marco));
            }
            return out_cmd;
        }
        public string LoadIncluded()
        {
            string out_cmd = "";
            foreach (string include in json_info.PROJECT_ADDITIONAL_INCLUDE_DIR)
            {
                var temp_path = Path.Join(PROJECT_ROOT_PATH,include);
                out_cmd += string.Format("-isystem \"{0}\" ", temp_path);
            }
            return out_cmd;
        }

        public List<string> GetSourceList() 
        {
            List<string> output = new List<string>();
            foreach (var source in json_info.PROJECT_SOURCE)
            {
                output.Add(Path.Join(PROJECT_ROOT_PATH, source));
            }
            return output;
        }

        public string GetProjectName() { return json_info.PROJECT_NAME; }

        public string GetProjectOutPath() { return Path.Join(PROJECT_ROOT_PATH, json_info.PROJECT_OUTPUT_DIR); }

        public string GetOptimizationLevel() { return json_info.PROJECT_OPTIMIZATION_LEVEL; }
    }
}
