#!/bin/sh
#
# Example job submission script.
# This example job simply invokes the 'sleep' command. Real jobs
# will want to do something much more exciting!
#
# Arguments for the qsub command are provided by the lines starting
# with special comments '#$' below. Full details can be found in the
# manual page for qsub ("man qsub"). The ones below are generally
# useful as a starting point.
#
# The following argument runs your job from the same directory this
# script lives in. This usually makes things easier for you.
#-cwd
#cd "$PBS_O_WORKDIR" || exit $?
#
# The next argument specifies a name for your job so that you can
# easily keep track of it.
# It usually isn't necessary, but worth doing it anyway.
# -N LRDP 
 
#
# The next argument will cause the system to send you emails when
# your job starts/finishes. This can be quite useful. The emails will
# go to <UUN>@ed.ac.uk. If you'd prefer to have the emails sent to
# another address, you can use the -M argument to specify this address.
#$ -m be
#
# It's good to get in the habit of setting a maximum run time for your
# job. This can help the scheduler determine when your job can be run.
# If your job exceeds your maximum run time, or that imposed by the queue,
# then your job will be killed.
# In this example, we're setting a maximum run time of 5 minutes.
#
########################################################################

export OMP_NUM_THREADS=${NSLOTS}
export OMP_PROC_BIND=true

# Now you set up and run your job. For this trivial job, we're simply
# going to go to sleep for a while. The "result" for this job will be the
# two messages we echo below.
echo "START"
echo $OMP_NUM_THREADS
#python3 runnerP.py 0

python3 runner.py 10 0.37375 0.3740 0.000125 5000000 500 1 5000 0

