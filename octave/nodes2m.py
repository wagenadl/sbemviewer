#!/usr/bin/python3

import sqlite3

######################################################################
def getpoints(q, tid, name, typ):
    print("%s{%i} = [" % (name, tid))
    for row in q.execute("select x, y, z from nodes"
                         " where tid==%i and typ==%i" % (tid, typ)):
        print(row[0], row[1], row[2])
    print("];")
    
def gettree(q, tid):
    print("tree{%i} = [" % tid)
    for row in q.execute("select n1.x, n1.y, n1.z, n2.x, n2.y, n2.z"
              " from nodecons as nc"
              " inner join nodes as n1 on n1.nid==nc.nid1"
              " inner join nodes as n2 on n2.nid==nc.nid2"
              " where n1.tid==%i and n1.nid<n2.nid" % tid):
        print(row[0], row[1], row[2], row[3], row[4], row[5])
    print("];")

db = sqlite3.connect('/home/wagenaar/Desktop/170428_ganglion10.sbemdb');
q = db.cursor()

print("postsyn = {};")
print("presyn = {};")
print("tree = {};")
print("soma = {};")
print("exitpoint = {};")
print("tids = [];")
print("tname = {};")

tids = []
for row in q.execute("select tid,tname from trees"):
    print("tids(end+1) = %i;" % row[0])
    print('tname{%i} = "%s";' % (row[0], row[1]))
    tids.append(row[0])

for tid in tids:
    gettree(q, tid)
    getpoints(q, tid, 'soma', 1)
    getpoints(q, tid, 'exitpoint', 2)
    getpoints(q, tid, 'presyn', 5)
    getpoints(q, tid, 'postsyn', 6)


