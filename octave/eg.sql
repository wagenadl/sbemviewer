-- Select all tree IDs that have any synaptic connections onto tree #515 along with their synapse counts
select n.tid, count(*) from syncons as sc inner join nodes as n on sc.nid==n.nid inner join synapses as s on sc.sid==s.sid inner join syncons as sc2 on sc2.sid==s.sid inner join nodes as n2 on sc2.nid==n2.nid where n2.tid=515 and n.typ==5 group by n.tid;


-- Select nodes on tree #409 that are presynaptic to tree #515
select n.* from syncons as sc inner join nodes as n on sc.nid==n.nid inner join synapses as s on sc.sid==s.sid inner join syncons as sc2 on sc2.sid==s.sid inner join nodes as n2 on sc2.nid==n2.nid where n.tid==409 and n2.tid=515 and n.typ==5;

