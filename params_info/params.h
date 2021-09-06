#pragma once
#include <sstream>

#include <pe_sieve_types.h>
#include <paramkit.h>

#include "pe_sieve_params_info.h"
#include "../term_util.h"

using namespace paramkit;
using namespace pesieve;

//scan options:
//scan options:
#define PARAM_IAT "iat"
#define PARAM_HOOKS "hooks"
#define PARAM_SHELLCODE "shellc"
#define PARAM_DATA "data"
#define PARAM_MODULES_IGNORE "mignore"
#define PARAM_PROCESSES_IGNORE "pignore"
#define PARAM_PNAME "pname"
#define PARAM_PID "pid"
#define PARAM_LOOP "loop"
#define PARAM_REFLECTION "refl"
#define PARAM_DOTNET_POLICY "dnet"
#define PARAM_PTIMES "ptimes"

//dump options:
#define PARAM_IMP_REC "imp"
#define PARAM_DUMP_MODE "dmode"

//output options:
#define PARAM_QUIET "quiet"
#define PARAM_OUT_FILTER "ofilter"
#define PARAM_SUSPEND "suspend"
#define PARAM_KILL "kill"
#define PARAM_UNIQUE_DIR "uniqd"
#define PARAM_DIR "dir"
#define PARAM_MINIDUMP "minidmp"
#define PARAM_LOG "log"
#define PARAM_JSON "json"
#define PARAM_JSON_LVL "jlvl"

//info:
#define PARAM_DEFAULTS "default"

#define INFO_SPACER "\t   "

class HHParams : public Params
{
public:
    HHParams(const std::string &version)
        : Params(version)
    {
        {
            std::stringstream ss1;
            ss1 << "Scan only processes with given PIDs (dec or hex, separated by '" << PARAM_LIST_SEPARATOR << "').";
            std::stringstream ss2;
            ss2 << INFO_SPACER << "Example: 5367" << PARAM_LIST_SEPARATOR << "0xa90";
            this->addParam(new StringParam(PARAM_PID, false));
            this->setInfo(PARAM_PID, ss1.str(), ss2.str());
        }
        {
            std::stringstream ss1;
            ss1 << "Scan only processes with given names (separated by '" << PARAM_LIST_SEPARATOR << "').";
            std::stringstream ss2;
            ss2 << INFO_SPACER << "Example: iexplore.exe" << PARAM_LIST_SEPARATOR << "firefox.exe";
            this->addParam(new StringParam(PARAM_PNAME, false));
            this->setInfo(PARAM_PNAME, ss1.str(), ss2.str());
        }
        {
            std::stringstream ss1;
            ss1 << "Make a unique, timestamped directory for the output of each scan.";
            std::stringstream ss2;
            ss2 << INFO_SPACER << "Prevents overwriting results from previous scans.";
            this->addParam(new BoolParam(PARAM_UNIQUE_DIR, false));
            this->setInfo(PARAM_UNIQUE_DIR, ss1.str(), ss2.str());
        }
        {
            std::stringstream ss1;
            ss1 << "Do not scan process/es with given name/s (separated by '" << PARAM_LIST_SEPARATOR << "').";
            std::stringstream ss2;
            ss2 << INFO_SPACER << "Example: explorer.exe" << PARAM_LIST_SEPARATOR << "conhost.exe";
            this->addParam(new StringParam(PARAM_PROCESSES_IGNORE, false));
            this->setInfo(PARAM_PROCESSES_IGNORE, ss1.str(), ss2.str());
        }

        this->addParam(new IntParam(PARAM_PTIMES, false, IntParam::INT_BASE_DEC));
        this->setInfo(PARAM_PTIMES, "Scan only processes created N seconds before HH, or later.");

        this->addParam(new BoolParam(PARAM_SUSPEND, false));
        this->setInfo(PARAM_SUSPEND, "Suspend processes detected as suspicious.");

        this->addParam(new BoolParam(PARAM_LOG, false));
        this->setInfo(PARAM_LOG, "Append each scan summary to the log.");

        this->addParam(new BoolParam(PARAM_KILL, false));
        this->setInfo(PARAM_KILL, "Kill processes detected as suspicious.");

        this->addParam(new BoolParam(PARAM_HOOKS, false));
        this->setInfo(PARAM_HOOKS, "Detect inline hooks and in-memory patches.");

        this->addParam(new BoolParam(PARAM_LOOP, false));
        this->setInfo(PARAM_LOOP, "Enable continuous scanning.");

        EnumParam *enumParam = new EnumParam(PARAM_IMP_REC, "imprec_mode", false);
        if (enumParam) {
            this->addParam(enumParam);
            this->setInfo(PARAM_IMP_REC, "Set in which mode the ImportTable should be recovered");
            for (size_t i = 0; i < PE_IMPREC_MODES_COUNT; i++) {
                t_imprec_mode mode = (t_imprec_mode)(i);
                enumParam->addEnumValue(mode, translate_imprec_mode(mode));
            }
        }

        enumParam = new EnumParam(PARAM_OUT_FILTER, "ofilter_id", false);
        if (enumParam) {
            this->addParam(enumParam);
            this->setInfo(PARAM_OUT_FILTER, "Filter the dumped output.");
            for (size_t i = 0; i < OUT_FILTERS_COUNT; i++) {
                t_output_filter mode = (t_output_filter)(i);
                enumParam->addEnumValue(mode, translate_out_filter(mode));
            }
        }

        this->addParam(new StringParam(PARAM_MODULES_IGNORE, false));
        {
            std::stringstream ss1;
            ss1 << "Do not scan module/s with given name/s (separated by '" << PARAM_LIST_SEPARATOR << "').";
            std::stringstream ss2;
            ss2 << "\t   Example: kernel32.dll" << PARAM_LIST_SEPARATOR << "user32.dll";
            this->setInfo(PARAM_MODULES_IGNORE, ss1.str(), ss2.str());
        }

        this->addParam(new BoolParam(PARAM_QUIET, false));
        this->setInfo(PARAM_QUIET, "Print only the summary. Do not log on stdout during the scan.");

        this->addParam(new BoolParam(PARAM_JSON, false));
        this->setInfo(PARAM_JSON, "Print the JSON report as the summary.");
        //
        //PARAM_JSON_LVL
        enumParam = new EnumParam(PARAM_JSON_LVL, "json_lvl", false);
        if (enumParam) {
            this->addParam(enumParam);
            this->setInfo(PARAM_JSON_LVL, "Level of details of the JSON report.");
            for (size_t i = 0; i < JSON_LVL_COUNT; i++) {
                t_json_level mode = (t_json_level)(i);
                enumParam->addEnumValue(mode, translate_json_level(mode));
            }
        }

        this->addParam(new BoolParam(PARAM_MINIDUMP, false));
        this->setInfo(PARAM_MINIDUMP, "Create a minidump of the full suspicious process.");

        //PARAM_SHELLCODE
        this->addParam(new BoolParam(PARAM_SHELLCODE, false));
        this->setInfo(PARAM_SHELLCODE, "Detect shellcode implants. (By default it detects PE only).");

        //PARAM_REFLECTION
        this->addParam(new BoolParam(PARAM_REFLECTION, false));
        this->setInfo(PARAM_REFLECTION, "Make a process reflection before scan.", "\t   This allows i.e. to force-read inaccessible pages.");

        //PARAM_IAT
        enumParam = new EnumParam(PARAM_IAT, "iat_scan_mode", false);
        if (enumParam) {
            this->addParam(enumParam);
            this->setInfo(PARAM_IAT, "Scan for IAT hooks.");
            for (size_t i = 0; i < PE_IATS_MODES_COUNT; i++) {
                t_iat_scan_mode mode = (t_iat_scan_mode)(i);
                enumParam->addEnumValue(mode, translate_iat_scan_mode(mode));
            }
        }

        //PARAM_DOTNET_POLICY
        enumParam = new EnumParam(PARAM_DOTNET_POLICY, "dotnet_policy", false);
        if (enumParam) {
            this->addParam(enumParam);
            this->setInfo(PARAM_DOTNET_POLICY, "Set the policy for scanning managed processes (.NET).");
            for (size_t i = 0; i < PE_DNET_COUNT; i++) {
                t_dotnet_policy mode = (t_dotnet_policy)(i);
                enumParam->addEnumValue(mode, translate_dotnet_policy(mode));
            }
        }

        //PARAM_DATA
        enumParam = new EnumParam(PARAM_DATA, "data_scan_mode", false);
        if (enumParam) {
            this->addParam(enumParam);
            this->setInfo(PARAM_DATA, "Set if non-executable pages should be scanned.");
            for (size_t i = 0; i < PE_DATA_COUNT; i++) {
                t_data_scan_mode mode = (t_data_scan_mode)(i);
                enumParam->addEnumValue(mode, translate_data_mode(mode));
            }
        }

        //PARAM_DUMP_MODE
        enumParam = new EnumParam(PARAM_DUMP_MODE, "dump_mode", false);
        if (enumParam) {
            this->addParam(enumParam);
            this->setInfo(PARAM_DUMP_MODE, "Set in which mode the detected PE files should be dumped.");
            for (size_t i = 0; i < PE_DUMP_MODES_COUNT; i++) {
                t_dump_mode mode = (t_dump_mode)(i);
                enumParam->addEnumValue(mode,  translate_dump_mode(mode));
            }
        }
        //PARAM_DIR
        this->addParam(new StringParam(PARAM_DIR, false));
        this->setInfo(PARAM_DIR, "Set a root directory for the output (default: current directory).");

        //optional: group parameters
        std::string str_group = "7. output options";
        this->addGroup(new ParamGroup(str_group));
        this->addParamToGroup(PARAM_DIR, str_group);
        this->addParamToGroup(PARAM_JSON, str_group);
        this->addParamToGroup(PARAM_JSON_LVL, str_group);
        this->addParamToGroup(PARAM_OUT_FILTER, str_group);
        this->addParamToGroup(PARAM_LOG, str_group);
        this->addParamToGroup(PARAM_UNIQUE_DIR, str_group);

        str_group = "2. scanner settings";
        this->addGroup(new ParamGroup(str_group));
        this->addParamToGroup(PARAM_QUIET, str_group);
        this->addParamToGroup(PARAM_REFLECTION, str_group);
        this->addParamToGroup(PARAM_LOOP, str_group);

        str_group = "4. scan options";
        this->addGroup(new ParamGroup(str_group));
        this->addParamToGroup(PARAM_DATA, str_group);
        this->addParamToGroup(PARAM_IAT, str_group);
        this->addParamToGroup(PARAM_SHELLCODE, str_group);
        this->addParamToGroup(PARAM_HOOKS, str_group);

        str_group = "5. dump options";
        this->addGroup(new ParamGroup(str_group));
        this->addParamToGroup(PARAM_MINIDUMP, str_group);
        this->addParamToGroup(PARAM_IMP_REC, str_group);
        this->addParamToGroup(PARAM_DUMP_MODE, str_group);

        str_group = "3. scan exclusions";
        this->addGroup(new ParamGroup(str_group));
        this->addParamToGroup(PARAM_DOTNET_POLICY, str_group);
        this->addParamToGroup(PARAM_MODULES_IGNORE, str_group);
        this->addParamToGroup(PARAM_PROCESSES_IGNORE, str_group);

        str_group = "1. scan targets";
        this->addGroup(new ParamGroup(str_group));
        this->addParamToGroup(PARAM_PID, str_group);
        this->addParamToGroup(PARAM_PNAME, str_group);
        this->addParamToGroup(PARAM_PTIMES, str_group);

        str_group = "6. post-scan options";
        this->addGroup(new ParamGroup(str_group));
        this->addParamToGroup(PARAM_KILL, str_group);
        this->addParamToGroup(PARAM_SUSPEND, str_group);
    }

    void printBanner()
    {
        char logo2[] = ""
            "@@@  @@@  @@@@@@  @@@      @@@       @@@@@@  @@@  @@@  @@@  @@@@@@\n"
            "@@!  @@@ @@!  @@@ @@!      @@!      @@!  @@@ @@!  @@!  @@! !@@    \n"
            "@!@!@!@! @!@  !@! @!!      @!!      @!@  !@! @!!  !!@  @!@  !@@!! \n"
            "!!:  !!! !!:  !!! !!:      !!:      !!:  !!!  !:  !!:  !!      !:!\n"
            " :   : :  : :. :  : ::.: : : ::.: :  : :. :    ::.:  :::   ::.: : \n"
            "       @@@  @@@ @@@  @@@ @@@  @@@ @@@@@@@ @@@@@@@@ @@@@@@@        \n"
            "       @@!  @@@ @@!  @@@ @@!@!@@@   @!!   @@!      @@!  @@@       \n"
            "       @!@!@!@! @!@  !@! @!@@!!@!   @!!   @!!!:!   @!@!!@!        \n"
            "       !!:  !!! !!:  !!! !!:  !!!   !!:   !!:      !!: :!!        \n"
            "        :   : :  :.:: :  ::    :     :    : :: ::   :   : :       \n";
        char *logo = logo2;
        WORD logo_color = DARK_MAGENTA;

        WORD curr_color = 0;
        if (get_current_color(STD_OUTPUT_HANDLE, curr_color)) {
            WORD current_bg = GET_BG_COLOR(curr_color);
            if (current_bg == logo_color) {
                logo_color = MAKE_COLOR(CYAN, current_bg);
            }
        }
        WORD old_color = set_color(logo_color);
        std::cout << "\n" << logo << std::endl;
        set_color(old_color);
    }

    void fillStruct(t_hh_params &ps)
    {
        fillPEsieveStruct(ps.pesieve_args);
        bool hooks = false;
        copyVal<BoolParam>(PARAM_HOOKS, hooks);
        ps.pesieve_args.no_hooks = hooks ? false : true;

        copyVal<StringParam>(PARAM_PID, ps.pids);
        copyVal<StringParam>(PARAM_PNAME, ps.pname);
        copyVal<StringParam>(PARAM_PROCESSES_IGNORE, ps.pnames_ignored);

        copyVal<StringParam>(PARAM_DIR, ps.out_dir);
        copyVal<BoolParam>(PARAM_UNIQUE_DIR, ps.unique_dir);
        copyVal<BoolParam>(PARAM_SUSPEND, ps.suspend_suspicious);
        copyVal<BoolParam>(PARAM_KILL, ps.kill_suspicious);

        copyVal<BoolParam>(PARAM_LOOP, ps.loop_scanning);
        copyVal<BoolParam>(PARAM_LOG, ps.log);
        copyVal<IntParam>(PARAM_PTIMES, ps.ptimes);
    }

    protected:
        void fillPEsieveStruct(t_params &ps)
        {
            copyVal<EnumParam>(PARAM_IMP_REC, ps.imprec_mode);
            copyVal<EnumParam>(PARAM_OUT_FILTER, ps.out_filter);

            copyCStr<StringParam>(PARAM_MODULES_IGNORE, ps.modules_ignored, _countof(ps.modules_ignored));

            copyVal<BoolParam>(PARAM_QUIET, ps.quiet);
            copyVal<BoolParam>(PARAM_JSON, ps.json_output);

            copyVal<EnumParam>(PARAM_JSON_LVL, ps.json_lvl);

            copyVal<BoolParam>(PARAM_MINIDUMP, ps.minidump);
            copyVal<BoolParam>(PARAM_SHELLCODE, ps.shellcode);
            copyVal<BoolParam>(PARAM_REFLECTION, ps.make_reflection);

            copyVal<EnumParam>(PARAM_IAT, ps.iat);
            copyVal<EnumParam>(PARAM_DOTNET_POLICY, ps.dotnet_policy);
            copyVal<EnumParam>(PARAM_DATA, ps.data);
            copyVal<EnumParam>(PARAM_DUMP_MODE, ps.dump_mode);
        }
};
