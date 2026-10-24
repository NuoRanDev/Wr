import os
import platform
from pathlib import Path
from typing import Dict

# if platform or os is null
# auto choose platform or os
OS_NAME:str = ""
CPU_PLATFORM:str = ""
PROJECT_ROOT_PATH = Path(Path(Path(os.path.abspath(__file__)).parent).parent).parent
COMPILER:str = ""
OUT_DIR:str = "/out/app/"
DLL_EXTENSION:str = ""
LIB_EXTENSION:str = ""
EXE_EXTENSION:str = ""

# core
CORE_PROJECT_NAME:str = "wrcore"
CORE_PROJECT_ADD_INCLUDE_DIR:list[str] = [
    "/inc/",
    "/src/",
    "/3rd/inc/"
]
CORE_PROJECT_MARCO:Dict[str, Dict[str, list[str]]] = {
    "cpu":
    {
        "amd64":["USE_AVX",],
        "arrch64":["USE_NEON",],
    },
    "os":
    {
        "windows":[],
        "linux":[],
    },
}
CORE_PROJECT_SOURCE:list[str] = [
    "/src/core/*cpp",
    "/inc/core/dynamic_array/*cpp",
    "/inc/core/string/*cpp",
    "/inc/os/*cpp",
    "/inc/os/linux/*cpp",
    "/inc/os/windows/*cpp",
    "/inc/os/dev/*cpp",
    "/inc/os/dev/arm-neon/*cpp",
    "/inc/os/dev/avx/*cpp",
]
CORE_PROJECT_ADD_LIB_DIR:Dict[str, Dict[str, list[str]]] = {
    "windows": ["Shlwapi.lib", "Advapi32.lib", "Pathcch.lib", "mimalloc.lib"],
    "linux": ["libmimalloc.a"],
}

def init_compiler()-> bool:
    global OS_NAME
    global CPU_PLATFORM

    global COMPILER
    global DLL_EXTENSION
    global LIB_EXTENSION
    global EXE_EXTENSION
    if(len(OS_NAME) == 0):
        OS_NAME = platform.system()
    if(OS_NAME == "Windows"):
        COMPILER = "clang-cl"
        DLL_EXTENSION = "{0}.dll"
        LIB_EXTENSION = "{0}.lib"
        EXE_EXTENSION = "{0}.exe"
    elif(OS_NAME == "Linux"):
        COMPILER = "clang"
        DLL_EXTENSION = "lib{0}.so"
        LIB_EXTENSION = "lib{0}.a"
        EXE_EXTENSION = "{0}"
    else:
        print("NO SUPPORT THAT OS {0}".format(OS_NAME))
        return False
    if(len(CPU_PLATFORM) == 0):
        CPU_PLATFORM = platform.uname().machine
    
    if(CPU_PLATFORM == "amd64" or CPU_PLATFORM == "AMD64" or CPU_PLATFORM == "x86_64"):
        CPU_PLATFORM = "amd64"
    elif(CPU_PLATFORM == "arrch64" or CPU_PLATFORM == "arm64"):
        CPU_PLATFORM = "arrch64"
    else:
        print("NO SUPPORT THAT CPU {0}".format(CPU_PLATFORM))
        return False
    return True

def load_defined(defined:list[str])->str:
    out:str = ""
    if not defined:
        return out
    for cur_marco in defined:
        out = out + "-D{0} ".format(cur_marco)
    return out

def load_included(included:list[str])->str:
    out:str = ""
    if not included:
        return out
    for cur_inc in included:
        out = out + "-isystemF:\"{0}\" ".format(str(PROJECT_ROOT_PATH) + cur_inc)
    return out

def load_source(sources:list[str])->str:
    out:str = ""
    if not sources:
        return out
    for cur_src in sources:
        out = out + "\"{0}\" ".format(str(PROJECT_ROOT_PATH) + cur_src)
    return out

def build(out_type:str, project_name:str, sources:list[str], include:list[str], marcos:Dict[str, Dict[str, list[str]]])->bool:
    global OS_NAME
    global CPU_PLATFORM

    global COMPILER
    global DLL_EXTENSION
    global LIB_EXTENSION
    global EXE_EXTENSION

    print("-------build-------")
    print("project name\t:{0}".format(project_name))

    cmd:str = COMPILER + " "
    out_file:str = ""
    
    if(out_type =="EXE"):
        out_file = EXE_EXTENSION.format(project_name)
    elif(out_type =="LIB"):
        out_file = LIB_EXTENSION.format(project_name)
    elif(out_type =="DLL"):
        out_file = DLL_EXTENSION.format(project_name)
    else:
        print("NO OUT FILE TYPE")
        return False
    print("output file\t:{0}".format(out_file))

    cmd = cmd + load_included(include)
    marco_list:list[str] = (marcos["os"]).get(OS_NAME)
    marco_list.extend((marcos["cpu"]).get(CPU_PLATFORM))
    
    cmd = cmd + load_source(sources)
    cmd = cmd + "-o {0}".format(out_file)
    print(cmd)

init_compiler()
build("DLL", CORE_PROJECT_NAME, CORE_PROJECT_SOURCE, CORE_PROJECT_ADD_INCLUDE_DIR, CORE_PROJECT_MARCO)