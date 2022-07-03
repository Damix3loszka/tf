set(template_path "/home/" $ENV{USER} "/")
file(WRITE /etc/tf.conf ${template_path})
file(MAKE_DIRECTORY ${template_path}.template_folder)