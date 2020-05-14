# code from: 
# https://interrupt.memfault.com/blog/automate-debugging-with-gdb-python-api
#
# for further information of GDB Commands In Python:
# https://sourceware.org/gdb/onlinedocs/gdb/Commands-In-Python.html#Commands-In-Python

from gdb.printing import PrettyPrinter, register_pretty_printer
import gdb
import uuid
from prettytable import PrettyTable

# We can use the gdb.parse_and_eval utility to convert arbitrary arguments passed 
# to the command into a gdb.Value. 
# From there we can index into C struct members to traverse the object. 
# In this example we will parse a UuidListNode pointer and following the next pointer 
# until we reach the end of the list. Let’s add the following 
# to our custom_gdb_extensions.py script:
class UuidListDumpCmd(gdb.Command):
    """Prints the ListNode from our example in a nice format!"""

    def __init__(self):
        # This registers our class as "uuid_list_dump"
        super(UuidListDumpCmd, self).__init__(
            "uuid_list_dump", gdb.COMMAND_USER
        )

    def _uuid_list_to_str(self, val):
        """Walk through the UuidListNode list.
        We will simply follow the 'next' pointers until we encounter NULL
        """

        # Initialize PrettyTable and add columns
        x = PrettyTable()
        x.field_names = ["Idx", "Addr", "UUID"]

        idx = 0
        node_ptr = val
        result = ""
        while node_ptr != 0:
            uuid = node_ptr["uuid"]

            # Add a new row for each UUID in the Linked List
            x.add_row([idx, str(node_ptr), str(uuid)])

            node_ptr = node_ptr["next"]
            idx += 1

        # Convert the table to a string and prepend the count string.
        result = x.get_string()
        result = ("Found a Linked List with %d nodes:\n" % idx) + result
        return result

    def complete(self, text, word):
        # We expect the argument passed to be a symbol so fallback to the
        # internal tab-completion handler for symbols
        return gdb.COMPLETE_SYMBOL

    def invoke(self, args, from_tty):
        # When we call "uuid_list_dump" from gdb, this is the method
        # that will be invoked.
        # We can pass args here and use Python CLI utilities like argparse
        # to do argument parsing
        print("Args Passed: %s" % args)

        node_ptr_val = gdb.parse_and_eval(args)
        if str(node_ptr_val.type) != "jmf::apps::gdbpy::UuidListNode *":
            print("Expected pointer argument of type (jmf::apps::gdbpy::UuidListNode *)")
            return

        print(self._uuid_list_to_str(node_ptr_val))

class UuidPrettyPrinter(object):
    """Print 'struct Uuid' as 'xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx'"""

    def __init__(self, val):
        self.val = val

    def to_string(self):
        # gdb.Value can be accessed just like they would be in C. Since
        # this is a Uuid struct we can index into "bytes" and grab the
        # value from each entry in the array
        array = self.val["bytes"]
        # Format the byte array as a hex string so Python uuid module can
        # be used to get the string
        uuid_bytes = "".join(
            ["%02x" % int(array[i]) for i in range(0, array.type.sizeof)]
        )
        return str(uuid.UUID(uuid_bytes))

class CustomPrettyPrinterLocator(PrettyPrinter):
    """Given a gdb.Value, search for a custom pretty printer"""

    def __init__(self):
        super(CustomPrettyPrinterLocator, self).__init__(
            "my_pretty_printers", []
        )

    def __call__(self, val):
        """Return the custom formatter if the type can be handled"""

        typename = gdb.types.get_basic_type(val.type).tag
        if typename is None:
            typename = val.type.name

        if typename == "jmf::apps::gdbpy::Uuid":
            return UuidPrettyPrinter(val)

# The replace=True is useful while developing a pretty printer because 
# when the script is re-source’d, 
# it will overwrite the previous pretty printer version
register_pretty_printer(None, CustomPrettyPrinterLocator(), replace=True)

UuidListDumpCmd()