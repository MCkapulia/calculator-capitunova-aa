import subprocess

def run_test(args, input_str, expected_returncode, expected_output=None, float_precision=None):
    
    res = subprocess.run(args, input=input_str, text=True, capture_output=True)

    actual_returncode = res.returncode
  
    if expected_returncode != 0:
       
        assert actual_returncode != 0, (
            f"Input: {input_str!r}\n"
            f"Expected ERROR code (not 0), but got: {actual_returncode}\n"
            f"Stdout: {res.stdout!r}\n"
            f"Stderr: {res.stderr!r}"
        )
    else:
      
        assert actual_returncode == 0, (
            f"Input: {input_str!r}\n"
            f"Expected return code 0 but got: {actual_returncode}\n"
            f"Stdout: {res.stdout!r}\n"
            f"Stderr: {res.stderr!r}"
        )

  
    if expected_output is None:
        return

   
    output = res.stdout.strip()

    if float_precision is not None:
       
        try:
            expected_val = round(float(expected_output), float_precision)
            output_val   = round(float(output), float_precision)
            assert output_val == expected_val, (
                f"Input: {input_str!r}\n"
                f"Expected float output: {expected_val:.{float_precision}f} but got: {output!r}"
            )
        except ValueError:
           
            assert False, f"Output is not a valid float: {output!r}"
    else:
      
        try:
            expected_val = int(expected_output)
            output_val   = int(output)
            assert output_val == expected_val, (
                f"Input: {input_str!r}\n"
                f"Expected integer output: {expected_val} but got: {output!r}"
            )
        except ValueError:
          
            assert False, f"Output is not a valid integer: {output!r}"


def test_great():
   
    run_test(["./build/app.exe"], "5+4", 0, "9")

def test_great_float():
   
    run_test(["./build/app.exe", "--float"], "4/5", 0, "0.8000", float_precision=4)

def test_priority():
   
    run_test(["./build/app.exe"], "6 + 2 * 4 * (6 + 5)", 0, "94")

def test_operator():
   
    run_test(["./build/app.exe"], "45 * 100 (34)", 1)

def test_minus_float():
   
    run_test(["./build/app.exe", "--float"], "50 * 3 + -4", 1)

def test_div_float():
   
    run_test(["./build/app.exe", "--float"], "20 + 101 / 3 / 2", 0, "36.8333", float_precision=4)

def test_symb():
    
    run_test(["./build/app.exe"], "t34 * 2", 1)

def test_parenthesis_float():
    
    run_test(["./build/app.exe", "--float"], "(3 + 2) /(5 - 3)", 0, "2.5000", float_precision=4)

def test_not_parenthesis_float():
    
    run_test(["./build/app.exe"], "(3 +2)/ (5 - 3", 1)

def test_double_operator():
   
    run_test(["./build/app.exe"], "23 + (75 ** 4)", 1)

def test_begining_operator():
  
    run_test(["./build/app.exe"], "*23 - 455 * 5", 1)



def test_diff_expr():
   
    run_test(
        ["./build/app.exe", "--float"],
        "21 * (17 + (48 - 97 / 2) * (3 + 4))",
        0,
        "283.5000",
        float_precision=4
    )

def test_arif_expr():
 
    expr = "1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 + 10 + 11 + 12 + 13+14 + 15 + 16 + 17 + 18	+ 19 + 20 - 100"
    run_test(["./build/app.exe"], expr, 0, "110")

def test_point_expr():

    run_test(["./build/app.exe"], "45.0 + 85", 1)

def test_very_expr():
    run_test(["./build/app.exe"], "((34 + 45) * 34 + (56 -	129) / 56 * 2", 1)


def all_tests():
    tests = [
        test_great,
        test_great_float,
        test_priority,
        test_operator,
        test_minus_float,
        test_div_float,
        test_symb,
        test_parenthesis_float,
        test_not_parenthesis_float,
        test_double_operator,
        test_begining_operator,
        test_diff_expr,
        test_arif_expr,
        test_point_expr,
        test_very_expr,
    ]
    for test in tests:
        test()
        print(f"{test.__name__} passed.")


if __name__ == "__main__":
    all_tests()
