#!/usr/bin/env python

import sys
import pprint

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
	print "Summarizing %s benchmarks..." % (pkg)
	pkg_sum = {}
	for record in records:
		name = record['name']
		del record['name']
		if name in pkg_sum:
			pkg_sum[name].append(record)
		else:
			pkg_sum[name] = [record]
	pp(pkg_sum)
