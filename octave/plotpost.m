function plotpost(tid, xf)
nodes;
[N D] = size(postsyn{tid});
pp1 = xf * postsyn{tid}(:,1:3)';
qmark(pp1(1,:), pp1(2,:));
end
