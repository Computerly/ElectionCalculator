# ****************************************************************************
# *                           Permutation Generator                          *
# *   Permutation Generator outputs every possible combination a person can  *
# *                    vote in an election of given length                   *
# ****************************************************************************
import itertools

def pad(s):
	global n
	zeros = '0' * (n - len(s))
	return s + zeros

def main():
	# request length
	# generate from 1 to n, format with 0's to fill to n
	# print list
	n = int(input("Number of Candidates\n>>> "))
	l = []
	dataset = [str(x) for x in range(1, n+1)]	# generate list of number 1 to n
	for i in range(1, n+2):
		l += itertools.permutations(dataset,i)	# add permutations of subset to list

	intList = []
	for x in l:
		tmp = ''.join(x).ljust(n, '0')	# join to string, pad with 0's to n length
		intList.append(int(tmp))		# Cast to int

	intList.sort()	# sort numerically
	print(intList)
	print(f"Length: {len(intList)}")

if __name__ == '__main__':
	main()
