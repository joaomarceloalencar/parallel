using System;
using System.Collections.Generic;

namespace SlurmMonitor
{
	public class Job
	{
		private string jobName;
		private int jobId;
		private string userName;
		private int userId;
		private string[] nodeList;

		public int JobId
		{
			get 
			{
				return jobId;
			}
		}

		public Job (string jobName, int jobId, string userName, int userId, string[] nodeList)
		{
			this.jobName = jobName;
			this.jobId = jobId;
			this.userName = userName;
			this.userId = userId;
			this.nodeList = nodeList;
		}

	}
}

