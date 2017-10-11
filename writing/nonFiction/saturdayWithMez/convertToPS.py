# this can be run from the command-line like this:
#   $ sketch --run-script convertToPS.py

import Sketch.Scripting
import Sketch.const
 
def doc_loaded(doc):
    print doc.meta.filename
context.main_window.Subscribe(Sketch.const.DOCUMENT, doc_loaded)
 
for number in [ "cover", "01", "02", "03", "04", "05", "06", "07", "08", "09",
                "10", "11", "12", "13", "14", "15", "16", "17", "18", "19",
                "20", "21", "22", "23", "24", "25", "26", "27", "28",
                "back_cover" ]:
    context.main_window.LoadFromFile( number + ".sk" )
    context.main_window.SavePS( number + ".eps" )
 
context.main_window.Exit()