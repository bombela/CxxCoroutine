#!/usr/bin/env python

import sys
import pprint
import matplotlib.pyplot as plt
import numpy as np
import math

pp = pprint.pprint

if len(sys.argv) != 2:
	print 'usage: %s logfile' % (sys.argv[0])
	exit(1)
logpath = sys.argv[1]

def stripunit(dic, key):
	dic[key] = dic[key].strip('nsec')

print "Loading %s..." % (logpath)

pkgs = {}
fd = open(logpath, 'r')
for line in fd:
	keyvals = [kv.strip().split('=') for kv in line.strip().split(',')]
	record = {}
	for kv in keyvals:
		record[kv[0]] = kv[1]
	stripunit(record, 'elapsed')
	stripunit(record, 'res')
	pkg = record['pkg']
	del record['pkg']
	del record['res']
	record['elapsed'] = float(record['elapsed']) / float(record['cnt'])
	del record['cnt']
	if pkg in pkgs:
		pkgs[pkg].append(record)
	else:
		pkgs[pkg] = [record]
fd.close()

for pkg, records in pkgs.items():
	print "Sorting %s benchmarks..." % (pkg)
	pkg_sum = {}
	for record in records:
		name = record['name']
		del record['name']
		if name in pkg_sum:
			pkg_sum[name].append(record)
		else:
			pkg_sum[name] = [record]
	pkgs[pkg] = pkg_sum

for pkg, records in pkgs.items():
	print "Computing average time for %s benchmarks..." % (pkg)
	for name, record in records.items():
		relto = record[0]['relto']
		elapsed = int(sum([r['elapsed'] for r in record]) / len(record))
		records[name] = { 'elapsed': elapsed, 'relto': relto }

def remove_rel_time(records, key):
	if 'relto' not in records[key]:
		return
	relto = records[key]['relto']
	#del records[key]['relto']
	if key == relto:
		records[key]['elapsed_abs'] = records[key]['elapsed']
		return

	if relto in records:
		remove_rel_time(records, relto)
		rel_records = records
	else:
		for p_name, p_records in pkgs.items():
			if relto in p_records:
				remove_rel_time(p_records, relto)
				rel_records = p_records
				break

	records[key]['elapsed_abs'] = int(records[key]['elapsed']
			- rel_records[relto]['elapsed'])

for pkg, records in pkgs.items():
	print "Removing relative time for %s benchmarks..." % (pkg)
	for name, record in records.items():
		print name, record
		remove_rel_time(records, name)

def computeSubplotColAndRow(nbsubfig):
	size = int(math.sqrt(nbsubfig - 1)) + 1
	nbrow = size
	nbcol = size
	empty = (nbrow * nbcol) - nbsubfig
	if empty >= size:
		nbcol -= (empty / size)
	return (nbrow, nbcol)

subrow, subcol = computeSubplotColAndRow(len(pkgs))
subIdx = 1

colors = ('b', 'g', 'y', 'r')
colorIdx = 0

plt.suptitle(logpath)

for pkg, records in pkgs.items():
	print "Plotting %s benchmarks..." % (pkg)

	records_keys = sorted(records.keys())

	plt.subplot(subrow * 100 + subcol * 10 + subIdx)
	pos = np.arange(len(records))+.5
	plt.barh(pos, [records[k]['elapsed'] for k in records_keys],
			align='center', color='y', alpha=0.15,
			label='total elapsed time')

	plt.barh(pos, [records[k]['elapsed_abs'] for k in records_keys],
			align='center', color='b', alpha=0.75,
			label='absolute elapsed time')

	plt.yticks(pos, records_keys)

	plt.xlabel('time in nanoseconds, shorter is better')

	plt.legend();
	colorIdx += 1
	subIdx += 1

print "Rendering..."
plt.show()
