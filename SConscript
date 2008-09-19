env = Environment()

# put other stuff in this list as needed.
sources = Split("""
src/jaspers.c
""")
object_list = env.Object(source = sources)

jasper = env.Program(target='jaspers', source=object_list)

Default( jaspers )

