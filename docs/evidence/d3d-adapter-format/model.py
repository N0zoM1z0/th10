"""Bounded abstract successful-display-query model. No native/API execution."""
def run(desktop,windowed,color,options,results,probe_result=0,proposed=False,no_vsync=True,flags=0x80):
 if desktop not in (22,23):raise ValueError('Finite ordinary formats only')
 chosen=desktop if windowed else (23 if options&1 else 22+int(color not in (-1,0)))
 if not windowed and color==-1 and not options&1:color=0
 refresh=0 if windowed or no_vsync else 60
 interval=0 if windowed else (0x80000000 if no_vsync else 1)
 swap=1;trace=[('display',desktop),('construct',chosen)];results=iter(results);retried=False
 flags|=2
 while True:
  hardware=False;success=False
  for kind,vp in ((('HAL',64),('HAL',32),('REF',32)) if not options&2 else (('REF',32),)):
   trace.append(('create',kind,vp,chosen,refresh,interval,swap))
   try:ok=next(results)
   except StopIteration:raise ValueError('Bounded creation outcomes exhausted')
   if ok:
    hardware=kind=='HAL';flags=(flags|1) if kind=='HAL' and vp==64 else flags&~1
    trace.append(('log',kind,vp));success=True;break
   if retried and kind=='HAL':trace.append(('log','unavailable',vp))
  if success:break
  if not no_vsync:
   trace.append(('log','refresh_failed'));refresh=0;retried=True
  elif interval==0x80000000:
   trace.append(('log','immediate_failed'));interval=1;swap=3
  else:
   trace += [('fatal',),('release_interface',)];return dict(trace=trace,flags=flags,options=options,color=color,return_value=1)
 trace += [('publish',chosen),('matrices',),('transforms',),('viewport',),('caps',),('warnings',)]
 if not options&1 and hardware:
  trace.append(('probe',('receiver',0,1,chosen if proposed else desktop,0,3,21)))
  if probe_result==0:flags|=4
  else:options|=1;flags&=~4;trace.append(('log','texture_warning'))
 trace += [('reset',),('screen_color',0xff000000),('closing',0),('last_render',0)]
 return dict(trace=trace,flags=flags,options=options,color=color,return_value=0)
