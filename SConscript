Import('env')
env.Replace(CPPPATH=['../include'])
obj=env.StaticObject(Glob('*.c'))
obj.append(env.StaticObject(Glob('*.s')))
Return('obj')



