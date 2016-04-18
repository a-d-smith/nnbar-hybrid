import sys
import ROOT

if len(sys.argv) < 2:
	msg  = '\n'
	msg += "Usage 1: %s $INPUT_ROOT_FILE(s)\n" % sys.argv[0]
	msg += '\n'
	sys.stderr.write(msg)
	sys.exit(1)

from larlite import larlite as fmwk

# Create ana_processor instance
my_proc = fmwk.ana_processor()

# Set input root file
#curPath = sys.argv[1];
#for x in xrange(100):
#  filename=curPath+'/larlite_mcinfo_{}.root'.format(x)
#  file = ROOT.TFile(filename)
#  if not file.IsZombie():
#    my_proc.add_input_file(filename)
#
for x in xrange(len(sys.argv)-1):
	file = ROOT.TFile(sys.argv[x+1])
	if not file.IsZombie():
		my_proc.add_input_file(sys.argv[x+1])


# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

# Specify output root file name
#my_proc.set_ana_output_file("hybridData.root");

# Attach an analysis unit ... here we use a base class which does nothing.
# Replace with your analysis unit if you wish.
# my_proc.add_process(fmwk.ana_base())
my_proc.add_process(fmwk.getData())

print
print  "Finished configuring ana_processor. Start event loop!"
print

# Let's run it.
my_proc.run();

# done!
print
print "Finished running ana_processor event loop!"
print

sys.exit(0)
