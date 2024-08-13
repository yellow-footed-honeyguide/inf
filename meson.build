project('file_info', 'c',
    version : '0.2.0',
    default_options : ['warning_level=3'])

magic_dep = dependency('libmagic')

src_files = [
    'src/main.c',
    'src/file_info.c',
    'src/utils.c',
    'src/handlers/text_handler.c',
    'src/handlers/image_handler.c',
    'src/handlers/video_handler.c',
    'src/handlers/pdf_handler.c',
    'src/handlers/archive_handler.c'
]

inc = include_directories('src')

executable('file_info',
    src_files,
    include_directories : inc,
    dependencies : magic_dep,
    install : true)

