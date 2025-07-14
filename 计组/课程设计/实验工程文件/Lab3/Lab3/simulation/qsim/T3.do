onerror {quit -f}
vlib work
vlog -work work T3.vo
vlog -work work T3.vt
vsim -novopt -c -t 1ps -L cycloneive_ver -L altera_ver -L altera_mf_ver -L 220model_ver -L sgate work.T3_vlg_vec_tst
vcd file -direction T3.msim.vcd
vcd add -internal T3_vlg_vec_tst/*
vcd add -internal T3_vlg_vec_tst/i1/*
add wave /*
run -all
