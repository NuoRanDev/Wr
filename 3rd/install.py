import shutil
import json
import os

GIT_CMD:str         = "git clone --depth=1 {0} /install/{1}"
CUR_PATH:str        = os.getcwd()
OUT_LIB_PATH:str    = "/bin/"
OUT_INC_PATH:str    = "/inc/"

class lib_info:
    lib_name:str
    library_url:str
    include_dir_path:str
    include_file_path:list[str]
    lib_path:str
    lib_file_name:dict
    build_cmd:dict
    make_cmd:dict
    def load(self, lib_project_name:str, json_segment:dict):
        self.lib_name           = lib_project_name
        self.library_url        = json_segment["library_url"]
        self.include_dir_path   = json_segment["include_dir_path"]
        self.include_file_path  = json_segment["include_file_path"]
        self.lib_path           = json_segment["lib_path"]
        self.lib_file_name      = json_segment["lib_file_name"]
        self.build_cmd          = json_segment["build_cmd"]
        self.make_cmd           = json_segment["make_cmd"]
        '''
        print("lib_name",self.lib_name)
        print("library_url",self.library_url)
        print("include_dir_path",self.include_dir_path)
        print("include_file_path",self.include_file_path)
        print("lib_path",self.lib_path)
        print("lib_file_name",self.lib_file_name)
        print("build_cmd",self.build_cmd)
        print("make_cmd",self.make_cmd)
        '''


def get_lib_info_list(json_file_name:str) -> list[lib_info] :
    file_path=os.path.join(sh_path , json_file_name)
    print("Load file : {0}".format(file_path))
    fs = open(file_path,"r")
    json_data=fs.read()
    fs.close()
    lib_infos:list[lib_info] = []
    json_segments:dict = json.loads(json_data)
    for lib_project_name,json_segment in json_segments.items():
        lib = lib_info()
        lib.load(lib_project_name,json_segment)
        lib_infos.append(lib)
    print("load end")
    return lib_infos

def start_build(this_lib_info:lib_info):
    run_cli:str              = ""
    default_project_path:str = CUR_PATH + "/install/" + this_lib_info.lib_name
    default_build_path:str   = default_project_path + "/build"
    default_lib_path:str     = default_project_path + this_lib_info.lib_path

    print("build {0}".format(this_lib_info.lib_name))
    print("git clone ...")
    run_cli = GIT_CMD.format(this_lib_info.library_url, this_lib_info.lib_name)
    #run(run_cli)
    print("git clone end")
    print("build and make binary")
    build_and_make(this_lib_info, default_lib_path, default_project_path, default_build_path, "Debug")
    build_and_make(this_lib_info, default_lib_path, default_project_path, default_build_path, "Release")


def build_and_make(this_lib_info:lib_info, default_lib_path:str, default_project_path:str, default_build_path:str,  BUILD_TYPE:str):
    print("build: {0}".format(BUILD_TYPE))
    
    run_cli:str = this_lib_info.build_cmd[BUILD_TYPE]
    run(run_cli.format(default_project_path,default_build_path))

    run_cli = this_lib_info.make_cmd[BUILD_TYPE]
    run(run_cli.format(default_build_path))
    for file in this_lib_info.lib_file_name[BUILD_TYPE]:
        to_copy(default_lib_path + file, CUR_PATH + OUT_LIB_PATH + BUILD_TYPE + "/" + file)
    _rm(default_build_path)



def run(cmd:str):
    print(cmd)
    os.system(cmd)

def to_copy(src:str, dst:str):
    print("{0} -> {1}".format(src, dst))
    shutil.copyfile(src, dst)

def _rm(path:str):
    print("delete {0}".format(path))
    shutil.rmtree(path)

sh_path = os.getcwd()
libs = get_lib_info_list("install_third_lib.json")
for lib in libs:
    start_build(lib)