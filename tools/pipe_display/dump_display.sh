cat /sys/kernel/debug/dri/0/i915_display_info

#############
### Global ##
intel_reg_read 0x1E1230 -c 1

#############
### PIPEA ###

echo "PIPE A"
# SPA color correction 0
intel_reg_read 0x1F21D0 -c 2
# SPA GAMA correction
intel_reg_read 0x1F21F4 -c 6
# PIPE GAMMA/CSC control
intel_reg_read 0x1E7A00 -c 1
# PIPE CONTROL
intel_reg_read 0x1F0008 -c 1
# PIPE image enhancement histogram control
intel_reg_read 0x1E1260 -c 1

#############
### PIPEB ###
echo "PIPE B"
# SPC color correction 0
intel_reg_read 0x1F23D0 -c 2
# SPC GAMA correction
intel_reg_read 0x1F23F4 -c 6
# PIPE GAMMA/CSC control
intel_reg_read 0x1E9A00 -c 1
# PIPE CONTROL
intel_reg_read 0x1F1008 -c 1
# PIPE image enhancement histogram control
intel_reg_read 0x1E1360 -c 1


#############
### PIPEC ###
echo "PIPE C"
# SPE color correction 0
intel_reg_read 0x1F25D0 -c 2
# SPE GAMA correction
intel_reg_read 0x1F25F4 -c 6
# PIPE GAMMA/CSC control
intel_reg_read 0x1EBA00 -c 1
# PIPE CONTROL
intel_reg_read 0x1F4008 -c 1



