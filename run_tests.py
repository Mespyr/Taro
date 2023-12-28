import os
import subprocess as sp

GENFILE_WITH_RUN_OUTPUT = lambda comp_proc, run_proc: f"""COMPILE: {comp_proc.returncode}
STDOUT:
{comp_proc.stdout}
STDERR:
{comp_proc.stderr}
RUN: {run_proc.returncode}
STDOUT:
{run_proc.stdout}
STDERR:
{run_proc.stderr}"""

GENFILE_COMP_ERROR = lambda comp_proc: f"""COMPILE: {str(comp_proc.returncode)}
STDOUT:
{comp_proc.stdout}
STDERR:
{comp_proc.stderr}"""

def main():
	_test_files = os.listdir("test")
	test_files = []
	for i in _test_files:
		if i.endswith(".rmbt"):
			test_files.append(i)

	expected_output_files = os.listdir("test/output")

	passed = 0
	failed = 0
	new = 0
	not_saved = 0

	for test in test_files:
		# generate output based off of test code
		genfile = ""
		test_base_name = test.removesuffix(".rmbt")
		comp_proc = sp.run(["./rambutan", "test/" + test], text = True, stdout = sp.PIPE, stderr = sp.PIPE)
		# run compiled test code
		if comp_proc.returncode == 0:
			run_proc = sp.run("./a.out", text = True, stdout = sp.PIPE, stderr = sp.PIPE)
			genfile = GENFILE_WITH_RUN_OUTPUT(comp_proc, run_proc)
		else:
			genfile = GENFILE_COMP_ERROR(comp_proc)

		# if there is any existing test file
		if test_base_name in expected_output_files:
			with open("test/output/" + test_base_name, "r") as eofile:
				if eofile.read() != genfile:
					print(f"test \033[1;34m'{test_base_name}' \033[1;31mfailed")
					os.system("tput sgr0")
					print(genfile)
					check = input(f"Would you like to update the output file for test '{test_base_name}'? (y/n) ")
					if check.lower() == "y":
						new += 1
						with open("test/output/" + test_base_name, "w+") as outfile:
							outfile.write(genfile)
					else: failed += 1
				else:
					passed += 1
					print(f"test \033[1;34m'{test_base_name}' \033[1;32mpassed")
					os.system("tput sgr0")
		else:
			# save new test file
			print(genfile)
			save_ = input(f"Do you want to save this to 'test/output/{test_base_name}'? (y/n) ")
			if save_.lower() == "y":
				new += 1
				with open("test/output/" + test_base_name, "w+") as outfile:
					outfile.write(genfile)
			else: not_saved += 1

	print(f"\033[1;32m{passed} passed, \033[1;31m{failed} failed, \033[1;33m{new} new, \033[1;35m{not_saved} not saved")
	os.system("tput sgr0")
	os.system("make clean")

if __name__ == "__main__":
	main()
