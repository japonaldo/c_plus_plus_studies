# based on: https://stackoverflow.com/a/10775939

# TODO remove absolute path
source /home/fujioka/Projects/japonaldo/cpp_studies/dev/apps/gdb-py/resources/custom_gdb_extensions.py

set width 0
set height 0
set verbose off

b main.cpp:14
commands
    # it should print 0 nodes
    uuid_list_dump g_list_head
    continue
end

b main.cpp:16
commands
    # now it should print 10 nodes
    uuid_list_dump g_list_head
    quit
end

run
