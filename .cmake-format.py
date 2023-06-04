with section("parse"):
  additional_commands = {
  "cpmaddpackage":  {
        "kwargs": {
            "NAME": "*",
            "VERSION": "*",
            "GIT_REPOSITORY": "*",
            "GIT_TAG": "*",
            "URL": "*",
            "DOWNLOAD_ONLY": "*",
        }
    }
  }

with section("format"):
  line_width = 100
  tab_size = 2
  max_subgroups_hwrap = 2
  max_pargs_hwrap = 6
  max_rows_cmdline = 2
  dangle_parens = True
  dangle_align = "prefix"
  enable_sort = True
