resource "aws_s3_bucket" "supergoon_world_bucket" {
  bucket = "etf-external-site"
  tags = {
    Name = "Etf External Site Bucket"
  }
}
resource "aws_s3_bucket_ownership_controls" "s3_bucket_acl_ownership" {
  bucket = aws_s3_bucket.supergoon_world_bucket.id
  rule {
    object_ownership = "ObjectWriter"
  }
}

resource "aws_iam_user" "supergoon_world_user" {
  name = "etf-uploader"
}

resource "aws_s3_bucket_acl" "b_acl" {
  bucket = aws_s3_bucket.supergoon_world_bucket.id
  depends_on = [ aws_s3_bucket_ownership_controls.s3_bucket_acl_ownership ]
  acl    = "private"
}

resource "aws_iam_policy" "pipeline_access_policy" {
  name        = "etf_s3_upload_policy"
  description = "Policy for adding items to the supergoon world bucket."

  policy = jsonencode({
    Version = "2012-10-17",
    Statement = [{
      Effect = "Allow",
      Action = ["s3:PutObject", "s3:ListBucket"],
      Resource = [
        aws_s3_bucket.supergoon_world_bucket.arn,
        "${aws_s3_bucket.supergoon_world_bucket.arn}/*",
      ],
    }],
  })
}

resource "aws_iam_user_policy_attachment" "supergoon_world_attachment" {
  policy_arn = aws_iam_policy.pipeline_access_policy.arn
  user       = aws_iam_user.supergoon_world_user.name
}
