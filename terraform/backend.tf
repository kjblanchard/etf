terraform {
  backend "s3" {
    bucket = "supergoon-terraform-plans"
    key    = "escapeTheFate"
    region = "us-east-2"
  }
}
