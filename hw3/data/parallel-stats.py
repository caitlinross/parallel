import sys

filename = sys.argv[1]
out = open(filename[:-4]+"-speedup.dat", "w")
f = open(filename, "r")
first_line = True
base = 0.0
base_tasks = 0
for line in f:
    tokens = line.split(" ")
    if (first_line):
        first_line = False
        base = float(tokens[3])
        base_tasks = int(tokens[0])
    else:
        tasks = int(tokens[0])*int(tokens[1])
        speedup = base/float(tokens[3])
        eff = (speedup*base_tasks)/tasks
        print str(tasks) + " & " + "%.6f"%speedup + " & " + "%.6f"%eff + " \\\\ \\hline"
        out.write(str(tasks) + " %.6f"%speedup + "\n")

f.close()
out.close()
