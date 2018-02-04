#Variables
kernel_file='Luxur'
env=Environment()
env['CFLAGS']='  -Wall -fstrength-reduce -fomit-frame-pointer\
 -finline-functions -nostdinc -fno-builtin\
 -fno-stack-protector'
env['CCCOMSTR']='[C]  $SOURCES'
env['ASCOMSTR']='[as] $SOURCES'
env['LINKCOM']='ld -T link.ld -o $TARGET $SOURCES'
env['LINKCOMSTR']='[LD] $SOURCES > $TARGET'


debug = ARGUMENTS.get('debug', 0)
if int(debug):
	env.Append(CCFLAGS = ' -g')
	env['CCCOMSTR']='[C] [DBG] $SOURCES'
	env['ASCOMSTR']='[AS][DBG] $SOURCES'

lnkd = Builder(action = 'ld -T link.ld -o $TARGET $SOURCES')
env.Append(BUILDERS = {'Linkit' : lnkd})

Export('env')
objs=(SConscript(['boot/SConscript',
                  'kernel/SConscript',
		  'lib/SConscript'],'env'))


#linkeamos los Objetos en el kernel
env.Program(kernel_file,objs)
