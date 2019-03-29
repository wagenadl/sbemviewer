clear
pp = loadcsv('partners.csv');
tids = unique(sort(pp.SBEm_ID));

figure(1); clf
xf = [1 0 0; 0 1 0; 0 0 1];
h = plottree3(515, xf);
hold on
set(h, 'linewidth', 3, 'color', [.5 .5 .5]);
h = plotpost3(515, xf);
set(h, 'markersize', 8);

cc = distinctcolors(1000);


n = 0;
for tid = tids(:)'
  idx = find(pp.SBEm_ID==tid, 1);
  tn = pp.cCacocical_map{idx};
  tn = tn(2:end);
  idx = find(tn==' ', 1);
  if ~isempty(idx)
    tn = tn(1:idx-1);
  end
  if endswith(tn, 'R') || endswith(tn, 'L')
    tn = tn(1:end-1);
  end
  n = n + 1;
  h = plottree3(tid, xf);
  set(h, 'linewidth', 1, 'color', cc(n,:));
  h = plotsoma3(tid, xf, [' ' tn]);
  if ~isempty(h)
    set(h, 'color', cc(n,:));
    set(h(1), 'markersize', 18);
    set(h(2), 'fontsize', 12);
  end
end

axis off
set(gcf, 'paperposition', [1 1 6 5]);

n = 0;
for phi=0:.01:2*pi
  n++;
  view([cos(phi), sin(phi), 0]);
  axis([-300 300 -300 300 -300 200])
  print('-dpng', '-r300', sprintf('/tmp/%04i.png', n));
end
